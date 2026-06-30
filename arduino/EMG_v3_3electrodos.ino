// EMG_Nano.ino
// Lectura de sensor EMG V3 (3 electrodos) en Arduino Nano
// Muestreo con Timer1 a SAMPLE_RATE, cálculo de RMS en ventana móvil,
// envolvente por EMA y salida por Serial (CSV).

#include <Arduino.h>

const uint8_t SAMPLE_PIN = A0;
const unsigned long SAMPLE_RATE = 1000UL;    // Hz
const unsigned int WINDOW_MS = 50;           // ventana RMS en ms
const unsigned int PRINT_INTERVAL_MS = 50;   // cuánto enviamos por Serial
const float VREF = 5.0f;                     // ajustar a 3.3 si usas 3.3V Nano

// Buffer fijo para AVR (evita malloc). Máx 200 muestras (~200 ms a 1kHz)
const unsigned int MAX_WINDOW_SAMPLES = 200;
float bufferVals[MAX_WINDOW_SAMPLES];
unsigned int windowSize = 0;
unsigned int bufIndex = 0;
float sumSquares = 0.0f;

// Calibración (en unidades ADC: 0..1023)
float baseline = 0.0f;        // offset ADC
float maxCalibration = 200.0f; // RMS correspondiente a 100% (en ADC units)

// Timers/flags
volatile bool sampleFlag = false;
unsigned long lastPrintMs = 0;

void setupTimer1ForRate(unsigned long rateHz) {
  // Timer1 CTC mode, prescaler 8
  // OCR1A = (F_CPU / prescaler / rateHz) - 1
  const uint32_t prescaler = 8;
  uint32_t ocr = (F_CPU / prescaler / rateHz) - 1;
  if (ocr > 0xFFFF) ocr = 0xFFFF;

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= _BV(WGM12);       // CTC
  TCCR1B |= _BV(CS11);        // prescaler 8
  OCR1A = (uint16_t)ocr;
  TIMSK1 |= _BV(OCIE1A);      // Enable compare interrupt
  sei();
}

ISR(TIMER1_COMPA_vect) {
  sampleFlag = true;
}

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT); // AREF = Vcc (ajusta si usas referencia externa)
  pinMode(SAMPLE_PIN, INPUT);

  // calcular tamaño de ventana y limitar
  windowSize = max(1u, (unsigned int)((SAMPLE_RATE * WINDOW_MS) / 1000u));
  if (windowSize > MAX_WINDOW_SAMPLES) windowSize = MAX_WINDOW_SAMPLES;

  // inicializar buffer
  for (unsigned int i = 0; i < windowSize; ++i) bufferVals[i] = 0.0f;
  sumSquares = 0.0f;
  bufIndex = 0;

  Serial.println("EMG reader (Nano) inicializado");
  Serial.print("Sample rate: "); Serial.print(SAMPLE_RATE); Serial.println(" Hz");
  Serial.print("Window (ms): "); Serial.println(WINDOW_MS);
  Serial.print("Window size (samples): "); Serial.println(windowSize);
  Serial.print("Vref (V): "); Serial.println(VREF, 3);

  // calibraciones
  delay(200);
  calibrateBaseline(1000); // 1s para baseline
  Serial.print("Baseline medido (ADC): "); Serial.println(baseline, 2);

  Serial.println("Si quieres calibrar máximo, haz una contracción máxima ahora (5s)...");
  delay(500);
  calibrateMax(5000);
  Serial.print("Max calibration (ADC RMS estimate): "); Serial.println(maxCalibration, 2);

  // configurar Timer1
  setupTimer1ForRate(SAMPLE_RATE);
  lastPrintMs = millis();
}

void loop() {
  // cuando el timer marca, procesar una muestra
  if (sampleFlag) {
    sampleFlag = false;

    int raw = analogRead(SAMPLE_PIN); // 0..1023
    float centered = (float)raw - baseline;

    // actualizar buffer circular y sumSquares eficientemente
    float old = bufferVals[bufIndex];
    bufferVals[bufIndex] = centered;
    sumSquares = sumSquares - (old * old) + (centered * centered);

    bufIndex++;
    if (bufIndex >= windowSize) bufIndex = 0;
  }

  // salida periódica
  unsigned long msNow = millis();
  if (msNow - lastPrintMs >= PRINT_INTERVAL_MS) {
    lastPrintMs = msNow;
    float meanSquares = sumSquares / (float)windowSize;
    float rms = sqrt(meanSquares); // RMS en unidades ADC

    // Envolvente (EMA)
    static float envelope = 0.0f;
    const float alpha = 0.2f;
    envelope = alpha * rms + (1.0f - alpha) * envelope;

    // Normalizar a 0..100% usando maxCalibration (en ADC)
    float pct = 0.0f;
    if (maxCalibration > 1e-6f) {
      pct = (envelope / maxCalibration) * 100.0f;
      if (pct < 0.0f) pct = 0.0f;
      if (pct > 100.0f) pct = 100.0f;
    }

    // Convertir a voltios/mV
    float rmsV = (rms / 1023.0f) * VREF;
    float envV = (envelope / 1023.0f) * VREF;

    // Imprimir CSV: tiempo_ms, raw_rms_ADC, rms_V, envelope_V, pct
    Serial.print(msNow);
    Serial.print(',');
    Serial.print(rms, 2);
    Serial.print(',');
    Serial.print(rmsV * 1000.0f, 1); // mV
    Serial.print(',');
    Serial.print(envV * 1000.0f, 1); // mV
    Serial.print(',');
    Serial.println(pct, 1);
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

// Pide contracción máxima y estima RMS pico (en ADC units)
void calibrateMax(unsigned long dur_ms) {
  unsigned long end = millis() + dur_ms;
  float peak = 0.0f;
  while (millis() < end) {
    int raw = analogRead(SAMPLE_PIN);
    float centered = fabs((float)raw - baseline);
    if (centered > peak) peak = centered;
    delay(2);
  }
  // Ajuste empírico: RMS suele ser menor que pico, factor ~0.6
  maxCalibration = max(peak * 0.6f, 1.0f);
}
