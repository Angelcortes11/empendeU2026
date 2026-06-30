// EMG_v3_3electrodos.ino
// Lectura de salida analógica de un sensor EMG V3 (3 electrodos)
// Calcula RMS en ventana móvil y una envolvente; envía por Serial.
//
// Conexiones:
//  - Sensor VCC -> 5V (o 3.3V si el sensor lo requiere)
//  - Sensor GND -> GND
//  - Sensor OUT -> A0
//  - Electrodos: +, -, REF a la placa del sensor (no al Arduino directamente)
//
// Ajustes:
const uint8_t SAMPLE_PIN = A0;
const unsigned long SAMPLE_RATE = 1000UL; // Hz (puedes probar 500..2000)
const unsigned int WINDOW_MS = 50;        // tamaño de ventana RMS en ms (ej. 50 ms)
const unsigned int PRINT_INTERVAL_MS = 50; // intervalo para enviar datos al PC

// Calibración (se puede ajustar durante ejecución)
float baseline = 0.0;      // offset en unidades ADC (0..1023)
float maxCalibration = 200.0; // valor RMS correspondiente al 100% (ajustar con calibración)

// Variables internas
unsigned int windowSize;
float *bufferVals; // buffer circular de lectura (valores centrados en baseline)
unsigned int bufIndex = 0;
float sumSquares = 0.0;
unsigned long lastPrintMs = 0;

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT); // AREF = Vcc (ajusta si usas otra referencia)
  // Calcular windowSize y reservar buffer
  windowSize = max(1u, (unsigned int)((SAMPLE_RATE * WINDOW_MS) / 1000u));
  bufferVals = (float*)malloc(sizeof(float) * windowSize);
  if (!bufferVals) {
    Serial.println("ERROR: No hay memoria para el buffer");
    while (1);
  }
  for (unsigned int i = 0; i < windowSize; ++i) bufferVals[i] = 0.0;
  sumSquares = 0.0;
  bufIndex = 0;

  Serial.println("EMG reader inicializado");
  Serial.print("Sample rate: "); Serial.print(SAMPLE_RATE); Serial.println(" Hz");
  Serial.print("Window (ms): "); Serial.println(WINDOW_MS);
  Serial.print("Window size (samples): "); Serial.println(windowSize);

  // Opcional: medir baseline automático unos 1s al iniciar
  calibrateBaseline(1000);
  Serial.print("Baseline medido: "); Serial.println(baseline);
  Serial.println("Realiza una contracción máxima para calibrar maxCalibration (5s) o usa el valor por defecto.");
  delay(500);
  calibrateMax(5000);
  Serial.print("Max calibration: "); Serial.println(maxCalibration);
  Serial.println("Listo.");
}

void loop() {
  static unsigned long nextMicros = 0;
  unsigned long interval = 1000000UL / SAMPLE_RATE;
  if (nextMicros == 0) nextMicros = micros();

  // muestreo con temporización simple
  unsigned long now = micros();
  if (now >= nextMicros) {
    // leer ADC
    int raw = analogRead(SAMPLE_PIN); // 0..1023
    // centrar respecto a baseline y rectificar (EMG bipolar -> usamos valor signed alrededor de baseline)
    float centered = (float)raw - baseline;

    // actualizar buffer circular y sumSquares para RMS
    float old = bufferVals[bufIndex];
    bufferVals[bufIndex] = centered;
    // mantener sum of squares para RMS eficientemente
    sumSquares -= old * old;
    sumSquares += centered * centered;
    bufIndex++;
    if (bufIndex >= windowSize) bufIndex = 0;

    nextMicros += interval;
    // prevenir drift si se atrasa mucho
    if ((long)(micros() - nextMicros) > (long)interval) {
      nextMicros = micros() + interval;
    }
  }

  // cálculo y salida a intervalos regulares
  unsigned long msNow = millis();
  if (msNow - lastPrintMs >= PRINT_INTERVAL_MS) {
    lastPrintMs = msNow;
    float meanSquares = sumSquares / (float)windowSize;
    float rms = sqrt(meanSquares); // RMS en unidades ADC
    // opcional: aplicar rectificación/envolvente adicional con EMA
    static float envelope = 0.0;
    const float alpha = 0.2; // factor EMA 0..1 (mayor -> respuesta más rápida)
    envelope = alpha * rms + (1.0 - alpha) * envelope;

    // Normalizar a 0..100% con el valor maxCalibration (evitar división por 0)
    float pct = 0.0;
    if (maxCalibration > 1e-6) {
      pct = (envelope / maxCalibration) * 100.0;
      if (pct < 0.0) pct = 0.0;
      if (pct > 100.0) pct = 100.0;
    }

    // Enviar por serial (CSV: tiempo_ms, rms, envelope, pct)
    Serial.print(msNow);
    Serial.print(',');
    Serial.print(rms, 2);
    Serial.print(',');
    Serial.print(envelope, 2);
    Serial.print(',');
    Serial.println(pct, 1);
  }
}

// Mide baseline promedio durante dur_ms milisegundos
void calibrateBaseline(unsigned long dur_ms) {
  unsigned long end = millis() + dur_ms;
  unsigned long count = 0;
  unsigned long accum = 0;
  while (millis() < end) {
    accum += analogRead(SAMPLE_PIN);
    count++;
    delay(1); // lectura ~1kHz si dur_ms grande; sin precisión estricta aquí
  }
  if (count > 0) baseline = (float)accum / (float)count;
}

// Pide al usuario que haga una contracción máxima durante dur_ms y mide RMS pico
void calibrateMax(unsigned long dur_ms) {
  Serial.println("Comienza calibración máxima: mantén contracción máxima ahora...");
  unsigned long end = millis() + dur_ms;
  float peak = 0.0;
  // Lee continuamente y calcula RMS sobre la pequeña ventana actual (aprox.)
  while (millis() < end) {
    int raw = analogRead(SAMPLE_PIN);
    float centered = (float)raw - baseline;
    // medir absoluto puntual (no es RMS fino, pero sirve para estimar)
    float val = fabs(centered);
    if (val > peak) peak = val;
    delay(2);
  }
  // ajustar un poco (RMS suele ser menor que pico); usar factor empírico
  maxCalibration = max(peak * 0.6f, 1.0f); // factor 0.6 como referencia
}