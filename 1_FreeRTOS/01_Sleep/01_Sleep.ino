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


  // se mi sono svegliato per un interrupt di un pin (vale sia il primo metodo che il secondo)
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_EXT0 || wakeUpRsn == ESP_SLEEP_WAKEUP_EXT1) {WakeUp_Interrupt(&rtc);}

  // se mi sono svegliato causa mancanza di batteria riinizializzo l'rtc
  if (wakeUpRsn != ESP_SLEEP_WAKEUP_TIMER) {WakeUp_PowerLoss(&rtc);}






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

esp_sleep_wakeup_cause_t wakeup_reason;
wakeup_reason = esp_sleep_get_wakeup_cause();




// case ESP_SLEEP_WAKEUP_EXT0 : 
  /*
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_X, level)

  This function accepts as first argument the pin you want to use, 
  in this format GPIO_NUM_X, in which X represents the GPIO number of that pin.

  The second argument, level, can be either 1 or 0. This represents the state of the GPIO that will trigger wake up.
  */

// case ESP_SLEEP_WAKEUP_EXT1 : 
/*
  esp_sleep_enable_ext1_wakeup(bitmask, mode)

ESP_EXT1_WAKEUP_ALL_HIGH -> questo dice se entrambi diventano alti
ESP_EXT1_WAKEUP_ANY_HIGH -> questo dice se uno solo dei due diventa alto
! fondamentale che entrambe le funzioni per il risveglio vengano chiamate prima che il dispositivo vada in sleep


ESEMPIO CREAZIONE BITMASK
Supponiamo che WAKEUP_PIN_1 = 33 e WAKEUP_PIN_2 = 34:
1ULL << 33: Risultato = 0x0000000200000000 (il 34° bit è 1).
1ULL << 34: Risultato = 0x0000000400000000 (il 35° bit è 1).
OR dei due valori:
0x0000000200000000 | 0x0000000400000000 = 0x0000000600000000
Quindi, 0x0000000600000000 è la bitmask risultante, con il 34° e il 35° bit impostati a 1.


// Definiamo i pin per il wakeup esterno
#define WAKEUP_PIN_1 GPIO_NUM_33
#define WAKEUP_PIN_2 GPIO_NUM_34

  A bitmask of the GPIO numbers that will cause the wake up;
  Mode: the logic to wake up the ESP32. It can be:
  ESP_EXT1_WAKEUP_ALL_LOW: wake up when all GPIOs go low;
  ESP_EXT1_WAKEUP_ANY_HIGH: wake up if any of the GPIOs go high.

  Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.

  // Configuriamo il wakeup tramite i pin 33 e 34
  // La modalità OR_WAKE indicherà che l'ESP32 si risveglierà se uno dei due pin diventa alto
  esp_sleep_enable_ext1_wakeup((1ULL << WAKEUP_PIN_1) | (1ULL << WAKEUP_PIN_2), ESP_EXT1_WAKEUP_ANY_HIGH);

*/

// case ESP_SLEEP_WAKEUP_TIMER : 
/*
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

*/      



