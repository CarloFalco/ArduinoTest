#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */

/*
RTC_DATA_ATTR int wakeUpCount = 0;
RTC_DATA_ATTR uint8_t needsToStayActive = 0;
RTC_DATA_ATTR  unsigned long lastSleep = 1704067200; //  January 1, 2024 12:00:00 AM
RTC_DATA_ATTR int bootCount = 0;
*/


#include "esp_sleep.h"
#include "driver/rtc_io.h"


// Definiamo il tempo di wakeup in microsecondi (60000 microsecondi = 60 millisecondi)
#define TIME_TO_SLEEP  10  // 60ms


void setup() {
  Serial.begin(115200);

  // Configuriamo il wakeup tramite timer
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);


  Serial.println("Entrando in modalità sleep...");
  delay(500);  // Diamo tempo al messaggio di essere inviato prima di dormire
  
  // Entriamo in modalità deep sleep
  esp_deep_sleep_start();
}



void loop() {
  // Questo codice non verrà eseguito, poiché l'ESP32 si risveglia dal deep sleep e ricomincia da setup().
}

