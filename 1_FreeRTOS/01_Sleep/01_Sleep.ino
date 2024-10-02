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
#define TIME_TO_SLEEP  20  // s

#define WAKEUP_PIN_1 GPIO_NUM_10  // Pin RTC 33
#define WAKEUP_PIN_2 GPIO_NUM_11  // Pin RTC 34



void setup() {
  pinMode(WAKEUP_PIN_1, INPUT_PULLDOWN);
  pinMode(WAKEUP_PIN_2, INPUT_PULLDOWN);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  esp_sleep_wakeup_cause_t wakeUpRsn = esp_sleep_get_wakeup_cause();  
  Serial.println("wakeup reason num: " + String(wakeUpRsn));
  // se mi sono svegliato per un interrupt di un pin (vale sia il primo metodo che il secondo)
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_EXT0 || wakeUpRsn == ESP_SLEEP_WAKEUP_EXT1) {WakeUp_Interrupt();}

  // se mi sono svegliato causa mancanza di batteria riinizializzo l'rtc
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_TIMER) {WakeUp_Timer();}

  // se mi sono svegliato causa mancanza di batteria riinizializzo l'rtc
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_UNDEFINED) {WakeUp_PowerLoss();}




  // Configuriamo il wakeup via pin
  esp_sleep_enable_ext1_wakeup((1ULL << WAKEUP_PIN_1) | (1ULL << WAKEUP_PIN_2), ESP_EXT1_WAKEUP_ANY_HIGH);

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


void WakeUp_Interrupt(void){
  Serial.println("Wake Up caused by interrupt");
}

void WakeUp_PowerLoss(void){
  Serial.println("Wake Up caused by powerloss");
}

void WakeUp_Timer(void){
  Serial.println("Wake Up caused by timer");
}