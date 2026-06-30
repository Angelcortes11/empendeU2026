// EMG_ESP32_with_Pot.ino
// EMG V3 (3 electrodos) en ESP32
// Muestreo con timer hardware a SAMPLE_RATE, RMS en ventana móvil, envolvente EMA.
// Potenciómetro en POT_PIN ajusta maxCalibration en tiempo real (normalización %).
//
// Conexiones sugeridas:
//  - Sensor VCC -> 3.3V (recomendado para ESP32)
//  - Sensor GND -> GND
//  - Sensor OUT -> GPIO34 (SAMPLE_PIN, ADC1_CH6, input-only)
//  - Pot: 3.3V --- POT --- GND, wiper -> GPIO35 (POT_PIN, ADC1_CH7)
//  - Electrodos -> placa EMG (E+, E-, REF)
//
// Nota: GPIO34/35 son ADC1 pins (input-only) y funcionan bien con analogRead en ESP32.

#include <Arduino.h>

const int SAMPLE_PIN = 34;         // ADC1_CH6 (input only)
const int POT_PIN = 35;            // ADC1_CH7 (input only)

const unsigned long SAMPLE_RATE = 1000UL;    // Hz
const unsigned int WINDOW_MS = 50;           // ventana RMS en ms
const unsigned int PRINT_INTERVAL_MS = 50;   // intervalo de impresión por Serial
const float VREF = 3.3f;                     // tensión de referencia (ESP32 => 3.3V)
const int ADC_MAX = 4095;                    // 12-bit ADC en ESP32

// Buffer fijo
const unsigned int MAX_WINDOW_SAMPLES = 2000; // suficiente para ventanas grandes
float bufferVals[MAX_WINDOW_SAMPLES];
unsigned int windowSize = 0;
unsigned int bufIndex = 0;
float sumSquares = 0.0f;

// Potenciómetro -> mapea a este rango (en unidades ADC) para maxCalibration
const float POT_MIN_CAL = 10.0f;    // mínimo valor de calibración (evita división por 0)
const float POT_MAX_CAL = 3500.0f;  // máximo (ajusta según amplitud esperada)

float baseline = 0.0f;        // offset ADC
float maxCalibration = 200.0f; // valor por defecto (se sobrescribe por pot en tiempo real)

// Timer/ISR flags
volatile bool sampleFlag = false;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer() {
  // marca que hay que tomar una muestra (no llamar analogRead desde ISR)
  portENTER_CRITICAL_ISR(&timerMux);
  sampleFlag = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setupTimerForRate(unsigned long rateHz) {
  // Timer hardware: prescaler y alarma para obtener la frecuencia deseada
  // Usamos timer 0, prescaler 80 -> tick = 1 microseg (80MHz / 80)
  const int timerNum = 0;
  const int prescaler = 80;
  timer = timerBegin(timerNum, prescaler, true); // countUp = true
  timerAttachInterrupt(timer, &onTimer, true);
  uint64_t alarmValue = 1000000UL / rateHz; // microsegundos por muestra
  timerAlarmWrite(timer, (uint32_t)alarmValue, true); // autoreload = true
  timerAlarmEnable(timer);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Configurar ADC ESP32
  analogSetWidth(12); // 12-bit: 0..4095
  analogSetPinAttenuation(SAMPLE_PIN, ADC_11db); // amplia rango cerca de 3.3V
  analogSetPinAttenuation(POT_PIN, ADC_11db);

  // Inicializar buffer
  windowSize = max(1u, (unsigned int)((SAMPLE_RATE * WINDOW_MS) / 1000u));
  if (windowSize > MAX_WINDOW_SAMPLES) windowSize = MAX_WINDOW_SAMPLES;
  for (unsigned int i = 0; i < windowSize; ++i) bufferVals[i] = 0.0f;
  sumSquares = 0.0f;
  bufIndex = 0;

  Serial.println("EMG ESP32 con Potenciometro - inicializando");
  Serial.print("Sample rate: "); Serial.print(SAMPLE_RATE); Serial.println(" Hz");
  Serial.print("Window (ms): "); Serial.println(WINDOW_MS);
  Serial.print("Window size (samples): "); Serial.println(windowSize);
  Serial.print("Vref (V): "); Serial.println(VREF, 3);

  // Calibración de baseline (sin contracción)
  delay(200);
  calibrateBaseline(1000); // 1s para baseline
  Serial.print("Baseline medido (ADC): "); Serial.println(baseline, 2);

  // Iniciar timer para muestreo
  setupTimerForRate(SAMPLE_RATE);
}

void loop() {
  // Procesar la marca de muestreo establecida por el timer ISR
  bool doSample = false;
  portENTER_CRITICAL(&timerMux);
  if (sampleFlag) {
    sampleFlag = false;
    doSample = true;
  }
  portEXIT_CRITICAL(&timerMux);

  if (doSample) {
    int raw = analogRead(SAMPLE_PIN); // 0..4095
    float centered = (float)raw - baseline;

    float old = bufferVals[bufIndex];
    bufferVals[bufIndex] = centered;
    sumSquares = sumSquares - (old * old) + (centered * centered);

    bufIndex++;
    if (bufIndex >= windowSize) bufIndex = 0;
  }

  // Salida periódica: cálculo y lectura del pot
  static unsigned long lastPrintMs = 0;
  unsigned long msNow = millis();
  if (msNow - lastPrintMs >= PRINT_INTERVAL_MS) {
    lastPrintMs = msNow;

    // Leer pot y mapear a rango de calibration (en ADC units)
    int potRaw = analogRead(POT_PIN); // 0..4095
    float potMapped = POT_MIN_CAL + ((float)potRaw / (float)ADC_MAX) * (POT_MAX_CAL - POT_MIN_CAL);
    maxCalibration = potMapped;

    float meanSquares = sumSquares / (float)windowSize;
    float rms = sqrt(meanSquares); // RMS en unidades ADC (0..4095)

    // Envolvente EMA
    static float envelope = 0.0f;
    const float alpha = 0.2f; // ajustar según preferencia
    envelope = alpha * rms + (1.0f - alpha) * envelope;

    // Normalizar usando maxCalibration
    float pct = 0.0f;
    if (maxCalibration > 1e-6f) {
      pct = (envelope / maxCalibration) * 100.0f;
      if (pct < 0.0f) pct = 0.0f;
      if (pct > 100.0f) pct = 100.0f;
    }

    // Convertir a voltios/mV (aprox. - ADC->VREF mapping)
    float rmsV = (rms / (float)ADC_MAX) * VREF;
    float envV = (envelope / (float)ADC_MAX) * VREF;

    // Imprimir CSV:
    // tiempo_ms, rms_ADC, rms_mV, env_mV, pct, pot_raw, pot_mapped
    Serial.print(msNow); Serial.print(',');
    Serial.print(rms, 2); Serial.print(',');
    Serial.print(rmsV * 1000.0f, 1); Serial.print(',');
    Serial.print(envV * 1000.0f, 1); Serial.print(',');
    Serial.print(pct, 1); Serial.print(',');
    Serial.print(potRaw); Serial.print(',');
    Serial.println(potMapped, 1);
  }
}

// Mide baseline promedio durante dur_ms milisegundos (en ADC units)
void calibrateBaseline(unsigned long dur_ms) {
  unsigned long end = millis() + dur_ms;
  unsigned long count = 0;
  unsigned long accum = 0;
  while (millis() < end) {
    accum += analogRead(SAMPLE_PIN);
    count++;
    delay(1);
  }
  if (count > 0) baseline = (float)accum / (float)count;
}
