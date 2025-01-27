#line 1 "D:\\Progetti\\ArduinoTest\\1_FreeRTOS\\03_FreeRTOS\\led2_sketch.h"
#include <Arduino.h>

#define LED_PIN 17

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */

#define TASK_MEDIUM  500   // 0.5 s
volatile int led2Status; 


extern TaskHandle_t task1Handle;

void led2Sketch(void *parameter)
{
    // qui inizializzo i parametri che mi servono per il task
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(TASK_MEDIUM); 

    pinMode(LED_PIN, OUTPUT);    


    for (;;)
    {
        led2Status = digitalRead(LED_PIN);
        digitalWrite(LED_PIN, !led2Status);
        
        vTaskDelayUntil(&lastWakeTime, period); 
    }
}

// Definizione della funzione di inizializzazione del task
void task_led_init(void) {
  xTaskCreate(led2Sketch, "taskName", 2048, NULL, 1, &task1Handle);
}



