#include <Arduino.h>

#define LED_PIN 18
#define PUSH_BUTTION_PIN 1

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */

#define TASK_MEDIUM  1*mS_TO_S_FACTOR   // 1 s

extern volatile int led2Status; 



void ledSketch(void *parameter)
{
    // qui inizializzo i parametri che mi servono per il task
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(TASK_MEDIUM); 

    pinMode(LED_PIN, OUTPUT);    


    for (;;)
    {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        // Serial.println(led2Status);

        vTaskDelayUntil(&lastWakeTime, period); 
    }
}


