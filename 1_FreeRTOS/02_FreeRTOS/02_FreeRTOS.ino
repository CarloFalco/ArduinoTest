#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */

#define TASK_FAST    100                // 0.1ms
#define TASK_MEDIUM  1*mS_TO_S_FACTOR   // 1 s
#define TASK_SLOW    10*mS_TO_S_FACTOR  // 10 s

// task 
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;
TaskHandle_t task3Handle;

// contatori dei task
int i=0,j=0,k=0;

// inizializzazione delle mie funzioni
void Scheduler_PwrOn(void);

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  // Crea i task
  Scheduler_PwrOn();
}


void loop() {

}

void task_fast(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_FAST);

  while (true) {
    // Esegui il codice del task 1
    i++; Serial.print("Task 1: \t i = ");Serial.println(i);

    vTaskDelayUntil(&lastWakeTime, period);
  }
}


void task_med(void* pvParameters) { 
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_MEDIUM);

  while (true) {
    // Esegui il codice del task 2
    j++; Serial.print("\t\t\t\tTask 2: \t j = ");Serial.println(j);

    vTaskDelayUntil(&lastWakeTime, period);
  }
}


void task_slow(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_SLOW);
  while (true) {
    // Esegui il codice del task 3
    k++; Serial.print("\t\t\t\t\t\t\t\tTask 3: \t k = ");Serial.println(k);

    vTaskDelayUntil(&lastWakeTime, period);
  }
}


// Creazione dei task //
void Scheduler_PwrOn(void) {
  xTaskCreatePinnedToCore(
    task_fast,         // Task function.
    "task_fast",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task1Handle,  // Task handle to keep track of created task
    0);            // pin task to core 0

  xTaskCreatePinnedToCore(
    task_med,         // Task function.
    "task_med",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task2Handle,  // Task handle to keep track of created task
    1);            // pin task to core 1

  xTaskCreatePinnedToCore(
    task_slow,         // Task function.
    "task_slow",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task3Handle,  // Task handle to keep track of created task
    1);            // pin task to core 1
                    
}
