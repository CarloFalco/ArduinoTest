#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//#define ENABLE_CALIBRATION
#include "calibration.h"

#define LED_BUILTIN 97

TaskHandle_t task1Handle;
TaskHandle_t task2Handle;



void setup() {
    Serial.begin(115200); // Inizializza la comunicazione seriale a 9600 baud
    Serial.println("Usa 'put nome valore' per aggiornare una variabile o 'get nome' per chiedere il valore attuale:");

    pinMode(LED_BUILTIN, OUTPUT);

    // Crea i task
    Scheduler_PwrOn();
}

void loop() {
    Serial.print("Valore di variabile1: ");
    Serial.println(variabili[0].valore);
    delay(1000);
    
}



void task_blink(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000);

  while (true) {
    // Esegui il codice del task 1
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    vTaskDelayUntil(&lastWakeTime, period);
  }
}



// Creazione dei task //
void Scheduler_PwrOn(void) {

  #ifdef ENABLE_CALIBRATION
  xTaskCreatePinnedToCore(
    task_calib,         // Task function.
    "task_calib",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task1Handle,  // Task handle to keep track of created task
    0);            // pin task to core 0        // pin task to core 0    
  #endif

  xTaskCreatePinnedToCore(
  task_blink,         // Task function.
  "task_blink",       // name of task.
  2048,          // Stack size of task
  NULL,          // parameter of the task
  1,             // priority of the task
  &task2Handle,  // Task handle to keep track of created task
  0);            // pin task to core 0        // pin task to core 0   
}


