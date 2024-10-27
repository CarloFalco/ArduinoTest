// TODO: inibire il risveglio dal pin di interrupt se si risveglia piu di 3 volte nello stesso periodo di powerlatch


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <ESP32Time.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"

#include "secret.h"
#include "Utilities.h"


#define S_TO_uS_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define S_TO_mS_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */
#define mS_TO_uS_FACTOR 1000ULL  /* Conversion factor for milli seconds to micro seconds */

#define TIME_TO_SLEEP  20  // s

#define WAKEUP_PIN_1 GPIO_NUM_10  // Pin RTC 33
#define WAKEUP_PIN_2 GPIO_NUM_11  // Pin RTC 34

#define LED_BUILTIN 97

#define TASK_FAST 500  // Periodo del task in millisecondi


RTC_DATA_ATTR int wakeUpCount = 0;
RTC_DATA_ATTR uint8_t needsToStayActive = 0;
RTC_DATA_ATTR unsigned long wakeUpPreviousTime = 0;

ESP32Time rtc(3600);  // Dichiarazione di rtc

TaskHandle_t task1Handle;


void setup() {
  // pin configuration
  pinMode(WAKEUP_PIN_1, INPUT_PULLDOWN);
  pinMode(WAKEUP_PIN_2, INPUT_PULLDOWN);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  Serial.print("Wakeup Count: ");  Serial.println(wakeUpCount);

  esp_sleep_wakeup_cause_t wakeUpRsn = esp_sleep_get_wakeup_cause();  

  Serial.println("wakeup reason num: " + String(wakeUpRsn));

  // se mi sono svegliato per un interrupt di un pin (vale sia il primo metodo che il secondo)
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_EXT0 || wakeUpRsn == ESP_SLEEP_WAKEUP_EXT1) {WakeUp_Interrupt();}

  // se mi sono svegliato causa mancanza di batteria riinizializzo l'rtc
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_TIMER) {WakeUp_Timer();}

  // se mi sono svegliato causa mancanza di batteria riinizializzo l'rtc
  if (wakeUpRsn == ESP_SLEEP_WAKEUP_UNDEFINED) {WakeUp_PowerLoss();}

}


void loop() {
  // Questo codice non verrà eseguito, poiché l'ESP32 si risveglia dal deep sleep e ricomincia da setup().

  if (needsToStayActive == 0){
    esp_sleep_wakeup_cause_t wakeUpRsn = esp_sleep_get_wakeup_cause();  
    uint64_t timeToNextWakeUp = TIME_TO_SLEEP * S_TO_uS_FACTOR;

    if (wakeUpRsn == ESP_SLEEP_WAKEUP_EXT0 || wakeUpRsn == ESP_SLEEP_WAKEUP_EXT1) {
      delay(1000);
      Serial.print("wakeUpPreviousTime: ");Serial.println(wakeUpPreviousTime);
      unsigned long tmp_1 = rtc.getEpoch() - wakeUpPreviousTime;
      unsigned long tmp_2 = tmp_1 * S_TO_uS_FACTOR;
      Serial.print("timeToNextWakeUp: ");Serial.print(timeToNextWakeUp);Serial.print("\t tmp_1: ");Serial.print(tmp_1);Serial.print("\t tmp_2: ");Serial.println(tmp_2);

      timeToNextWakeUp = timeToNextWakeUp - tmp_2;
      Serial.print("timeToNextWakeUp: ");Serial.println(timeToNextWakeUp);

    }

    if (wakeUpRsn == ESP_SLEEP_WAKEUP_TIMER) {
      digitalWrite(LED_BUILTIN, 0);
      vTaskDelete(task1Handle);
      task1Handle = NULL;  // Resetta l'handle dopo l'eliminazione
      wakeUpCount = 0;
      wakeUpPreviousTime = rtc.getEpoch() ;
      Serial.print("wakeUpPreviousTime: ");Serial.println(wakeUpPreviousTime);

    }
    if (wakeUpRsn == ESP_SLEEP_WAKEUP_UNDEFINED){
      wakeUpPreviousTime = rtc.getEpoch();
    }
    // digitalWrite(LED_BUILTIN, 0);
    // Configuriamo il wakeup via pin
    esp_sleep_enable_ext1_wakeup((1ULL << WAKEUP_PIN_1) | (1ULL << WAKEUP_PIN_2), ESP_EXT1_WAKEUP_ANY_HIGH);

    // Configuriamo il wakeup tramite timer
    esp_sleep_enable_timer_wakeup(timeToNextWakeUp);


    Serial.println("Entrando in modalità sleep...");
    Serial.flush();
    // Entriamo in modalità deep sleep
    esp_deep_sleep_start();
  }


} 

void WakeUp_PowerLoss(void){
  Serial.println("Wake Up caused by powerloss");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("ip: ");Serial.println(WiFi.localIP());

  // setup time
  setup_rtc_time(&rtc);


  String dataTime = rtc.getTime("%A, %B %d %Y %H:%M:%S");
  log_i("%s", dataTime.c_str());

}

void WakeUp_Interrupt(void){
  Serial.println("Wake Up caused by interrupt");
  wakeUpCount ++;
}

void WakeUp_Timer(void){
  Serial.println("Wake Up caused by timer");

  String dataTime = rtc.getTime("%A, %B %d %Y %H:%M:%S");
  log_i("%s", dataTime.c_str());
  
  xTaskCreate(led_blink_task, "LED blink task", 2048, NULL, 1, &task1Handle);   
  needsToStayActive = 1;
}


void led_blink_task(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_FAST);
  int count_iter = 0;
  while (true) {
    // Esegui il codice del task 1
    
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    count_iter ++;
    if (count_iter > 9){      
      needsToStayActive = 0;
    }

    vTaskDelayUntil(&lastWakeTime, period);
  }
}

