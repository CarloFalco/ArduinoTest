#line 1 "D:\\Progetti\\ArduinoTest\\1_FreeRTOS\\03_FreeRTOS\\03_FreeRTOS.ino"
#include <Arduino.h>

#include "led_sketch.h"    // Screen Sketch
#include "led2_sketch.h"    // Screen Sketch

/* prototipo di taks usando xTaskCreatePinnedToCore
xTaskCreatePinnedToCore(
    ledSketch, // Task function
    "taskName",   // Task name
    8192,         // Stack size
    NULL,         // Task input parameters
    1,            // Task priority, be carefull when changing this
    NULL,         // Task handle, add one if you want control over the task (resume or suspend the task)
    1             // Core to run the task on
);
*/
/* prototipo di taks usando xTaskCreate
xTaskCreate(
    led2Sketch, // Task function
    "taskName",   // Task name
    2048,         // Stack size
    NULL,         // Task input parameters
    1,            // Task priority, be carefull when changing this
    &task1Handle // Task handle, add one if you want control over the task (resume or suspend the task)
);

*/


 
TaskHandle_t task1Handle = NULL; // in questo modo posso sospendere il taks 

#line 33 "D:\\Progetti\\ArduinoTest\\1_FreeRTOS\\03_FreeRTOS\\03_FreeRTOS.ino"
void setup();
#line 48 "D:\\Progetti\\ArduinoTest\\1_FreeRTOS\\03_FreeRTOS\\03_FreeRTOS.ino"
void loop();
#line 33 "D:\\Progetti\\ArduinoTest\\1_FreeRTOS\\03_FreeRTOS\\03_FreeRTOS.ino"
void setup()
{
     

    Serial.begin(115200);
    xTaskCreatePinnedToCore(ledSketch, "taskName", 2048, NULL, 1, NULL, 1);
    task_led_init();

    // Attendere 5 secondi
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    Serial.println("Time to turn off led");
    // Cancellare il task
    vTaskDelete(task1Handle);
}

void loop()
{
}
