#define LED_BUILTIN 48
#define FROM_mm_to_m 0.001
#define FROM_ms_to_s 0.001

#include "Wind.h"


const float diameter = 80*2*FROM_mm_to_m; // Diametro del rotore in metri
const float pi = 3.14159;
const int numElements = 5; // Numero di elementi per la media mobile

#define TASK_ANEMOMETER 200  // Periodo del task in millisecondi

volatile int rotationCount = 0;
int rotationCounts[numElements] = {0, 0, 0, 0, 0}; // Array per memorizzare le ultime tre misurazioni di rotazioni
int ArrayIndex = 0;

const int AnemometerPin = 39; // Pin a cui è collegato il bottone

TaskHandle_t task1Handle;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.print("Diameter: " + String(diameter) + " [m]\n");

  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(AnemometerPin), countRevolutions, FALLING);

  xTaskCreate(anemometer_task, "evaluation of anemometer task", 2048, NULL, 1, &task1Handle);

}

void loop() {


}


void anemometer_task(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_ANEMOMETER);
  while (true) {
    // Aggiorna l'array delle rotazioni
    rotationCounts[ArrayIndex] = rotationCount;
    Serial.print("rotationCount: " + String(rotationCount) + "\n");
    ArrayIndex = (ArrayIndex + 1) % numElements; // Incrementa l'indice e lo resetta a 0 dopo 3 cicli

    // Calcola la media mobile delle rotazioni
    float sum = 0;
    for (int i = 0; i < numElements; i++) {
      sum += rotationCounts[i];
    }
    float averageRotationCountSum = sum/numElements;

    Serial.print("Media1: " + String(averageRotationCountSum) + "\n");

    // Calcola la frequenza media di rotazione
    float frequency = averageRotationCountSum / (TASK_ANEMOMETER / 1000.0); // Rotazioni per secondo
    // Calcola la circonferenza
    float circumference = pi * diameter;
    // Calcola la velocità del vento
    float windSpeed = frequency * circumference;

    Serial.print("Velocità del vento (media mobile): " + String(windSpeed) + " [m/s] \n");
    Serial.print("Velocità del vento (media mobile): " + String(windSpeed*3.6) + " [km/h] \n");
    Serial.print("Velocità del vento (media mobile): " + getDirection(windSpeed*3.6)+ "\n");

    // Resetta il contatore di rotazioni
    rotationCount = 0;
    vTaskDelayUntil(&lastWakeTime, period);
  }
}



void countRevolutions() {
  rotationCount++;
}

String getDirection(float windSpeed_kph) {
  // FONTE : https://it.wikipedia.org/wiki/Scala_di_Beaufort
  if (windSpeed_kph < 1) return "Calma";
  else if (windSpeed_kph >= 1 && windSpeed_kph < 6) return "Bava di vento";
  else if (windSpeed_kph >= 6 && windSpeed_kph < 11) return "Brezza leggera";
  else if (windSpeed_kph >= 11 && windSpeed_kph < 19) return "Brezza tesa";
  else if (windSpeed_kph >= 19 && windSpeed_kph < 29) return "Vento moderato";
  else if (windSpeed_kph >= 29 && windSpeed_kph < 39) return "Vento teso";
  else if (windSpeed_kph >= 39 && windSpeed_kph < 50) return "Vento fresco";
  else if (windSpeed_kph >= 50 && windSpeed_kph < 62) return "Vento forte";
  else if (windSpeed_kph >= 62 && windSpeed_kph < 75) return "Burrasca";
  else if (windSpeed_kph >= 75 && windSpeed_kph < 87) return "Burrasca forte";
  else if (windSpeed_kph >= 88 && windSpeed_kph < 102) return "Tempesta";
  else if (windSpeed_kph >= 102 && windSpeed_kph < 117) return "Tempesta violenta";
  else if (windSpeed_kph >= 117) return "Uragano";
  else return "Calma";
}



