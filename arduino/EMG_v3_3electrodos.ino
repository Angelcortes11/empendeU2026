// EMG_Nano_with_Pot_FIXED.ino
#include <Arduino.h>
#include <math.h>
#include <avr/interrupt.h>

const uint8_t SAMPLE_PIN = A0;
const uint8_t POT_PIN = A1;

// Configuración
const unsigned long SAMPLE_RATE = 1000UL;
const unsigned int WINDOW_MS = 50;
const unsigned int PRINT_INTERVAL_MS = 50;
const float VREF = 5.0f;

const unsigned int MAX_WINDOW_SAMPLES = 200;
const float POT_MIN_CAL = 10.0f;
const float POT_MAX_CAL = 800.0f;

// Variables
float bufferVals[MAX_WINDOW_SAMPLES];

unsigned int windowSize = 0;
unsigned int bufIndex = 0;

float sumSquares = 0.0f;
float baseline = 0.0f;
float maxCalibration = 200.0f;

volatile uint16_t sampleCount = 0;
unsigned long lastPrintMs = 0;

void setupTimer1ForRate(unsigned long rateHz);
void calibrateBaseline(unsigned long dur_ms);

void setupTimer1ForRate(unsigned long rateHz)
{
    const uint32_t prescaler = 8;
    uint32_t ocr = (F_CPU / prescaler / rateHz) - 1;

    if (ocr > 65535)
        ocr = 65535;

    cli();

    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;

    TCCR1B |= _BV(WGM12); // CTC
    TCCR1B |= _BV(CS11);  // prescaler 8

    OCR1A = (uint16_t)ocr;
    TIMSK1 |= _BV(OCIE1A);

    sei();
}

ISR(TIMER1_COMPA_vect)
{
    if (sampleCount < 65535)
        sampleCount++;
}

void setup()
{
    Serial.begin(115200);
    analogReference(DEFAULT);

    pinMode(SAMPLE_PIN, INPUT);
    pinMode(POT_PIN, INPUT);

    windowSize = (unsigned int)((SAMPLE_RATE * WINDOW_MS) / 1000UL);

    if (windowSize < 1)
        windowSize = 1;

    if (windowSize > MAX_WINDOW_SAMPLES)
        windowSize = MAX_WINDOW_SAMPLES;

    for (unsigned int i = 0; i < windowSize; i++)
        bufferVals[i] = 0.0f;

    Serial.println("================================");
    Serial.println(" EMG Nano Inicializado");
    Serial.println("================================");

    Serial.print("Sample Rate: ");
    Serial.println(SAMPLE_RATE);

    Serial.print("Window Samples: ");
    Serial.println(windowSize);

    delay(500);

    Serial.println("Calibrando baseline...");
    calibrateBaseline(3000);

    Serial.print("Baseline: ");
    Serial.println(baseline, 2);

    setupTimer1ForRate(SAMPLE_RATE);
    lastPrintMs = millis();
}

void loop()
{
    while (true)
    {
        noInterrupts();

        if (sampleCount == 0)
        {
            interrupts();
            break;
        }

        sampleCount--;
        interrupts();

        int raw = analogRead(SAMPLE_PIN);

        // Baseline dinámico (compensa drift)
        baseline = baseline * 0.999f + raw * 0.001f;

        float centered = (float)raw - baseline;

        float old = bufferVals[bufIndex];
        bufferVals[bufIndex] = centered;

        sumSquares -= old * old;
        sumSquares += centered * centered;

        if (sumSquares < 0.0f)
            sumSquares = 0.0f;

        bufIndex++;

        if (bufIndex >= windowSize)
            bufIndex = 0;
    }

    unsigned long now = millis();

    if (now - lastPrintMs >= PRINT_INTERVAL_MS)
    {
        lastPrintMs = now;

        int potRaw = analogRead(POT_PIN);

        maxCalibration =
            POT_MIN_CAL +
            ((float)potRaw / 1023.0f) *
            (POT_MAX_CAL - POT_MIN_CAL);

        float meanSquares = sumSquares / (float)windowSize;

        if (meanSquares < 0.0f)
            meanSquares = 0.0f;

        float rms = sqrt(meanSquares);

        static float envelope = 0.0f;
        const float alpha = 0.20f;

        envelope = alpha * rms + (1.0f - alpha) * envelope;

        float pct = 0.0f;

        if (maxCalibration > 0.0f)
        {
            pct = envelope * 100.0f / maxCalibration;

            if (pct < 0.0f)
                pct = 0.0f;

            if (pct > 100.0f)
                pct = 100.0f;
        }

        float rmsV = rms * VREF / 1023.0f;
        float envV = envelope * VREF / 1023.0f;

        Serial.print(now);
        Serial.print(",");

        Serial.print(rms, 2);
        Serial.print(",");

        Serial.print(rmsV * 1000.0f, 1);
        Serial.print(",");

        Serial.print(envV * 1000.0f, 1);
        Serial.print(",");

        Serial.print(pct, 1);
        Serial.print(",");

        Serial.print(potRaw);
        Serial.print(",");

        Serial.println(maxCalibration, 1);
    }
}

void calibrateBaseline(unsigned long dur_ms)
{
    unsigned long start = millis();
    unsigned long count = 0;
    unsigned long accum = 0;

    while (millis() - start < dur_ms)
    {
        accum += analogRead(SAMPLE_PIN);
        count++;
        delay(1);
    }

    if (count > 0)
        baseline = (float)accum / (float)count;
}
