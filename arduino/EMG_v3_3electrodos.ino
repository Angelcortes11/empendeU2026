#include <Arduino.h>

const int EMG_PIN = 34;      // Salida del EMG V3 conectada al GPIO34

const int ADC_MAX = 4095;

int baseline = 0;

// Valor que representa una contracción máxima.
// Ajustalo según las pruebas (800-1500 suele ser un buen comienzo).
const int MAX_CONTRACTION = 1000;

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(EMG_PIN, ADC_11db);

  Serial.println();
  Serial.println("=================================");
  Serial.println(" CALIBRANDO...");
  Serial.println(" Mantene el musculo relajado");
  Serial.println("=================================");

  delay(2000);

  long suma = 0;

  for (int i = 0; i < 1000; i++) {
    suma += analogRead(EMG_PIN);
    delay(2);
  }

  baseline = suma / 1000;

  Serial.print("Baseline: ");
  Serial.println(baseline);

  Serial.println();
  Serial.println("Comenza a contraer el musculo...");
  Serial.println();
}

void loop() {

  int lectura = analogRead(EMG_PIN);

  int actividad = lectura - baseline;

  if (actividad < 0)
    actividad = 0;

  int porcentaje = map(actividad, 0, MAX_CONTRACTION, 0, 100);

  porcentaje = constrain(porcentaje, 0, 100);

  Serial.print("Contraccion: ");

  Serial.print(porcentaje);

  Serial.print("%   ");

  Serial.print("Valor ADC: ");

  Serial.println(lectura);

  delay(50);
}
