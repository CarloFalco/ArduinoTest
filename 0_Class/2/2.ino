#define LED_BUILTIN 97
#define RGB_BRIGHTNESS 100

#include "Led.h"
#include "count.h"
#include <WiFi.h>
#include <ESP32Time.h>
#include "my_math.h"



const char *ssid = "AsusWifi_Smart";          // Change this to your WiFi SSID
const char *password = "E8H3EGV279";  // Change this to your WiFi password


//void wakeup_reason(void);
void isDST(ESP32Time *);
void changeColor(Led *);



class Led1 : public Led {
public:
    void yellow() {
      neopixelWrite(pin,64,64,0);
    }
};



Led1 led(LED_BUILTIN);
Count count(10);
ESP32Time rtc(0);  // offset in seconds GMT+1ù


void setup(){
  Serial.begin(115200);
  led.init();
  rtc.setTime(0, 0, 0, 1, 4, 2021);
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format

}


void loop(){
  delay(500);
  led.toggle();
  isDST(&rtc);
  delay(500);
  changeColor(&led);
  delay(500);
  led.yellow();
}


void changeColor(Led *led){
  led->red();
}

void isDST(ESP32Time *rtc) {
  // Regole per l'ora legale in Europa
  Serial.println(rtc->getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format
/*
  // L'ora legale inizia l'ultima domenica di marzo alle 2:00 AM e termina l'ultima domenica di ottobre alle 3:00 AM

  //int month = dt.month();
  //int day = dt.day();
  //int hour = dt.hour();
  //int dayOfWeek = dt.dayOfTheWeek();  // 0 = Sunday, 6 = Saturday


  // Controlla se siamo in marzo
  if (month == 3) {
    // Calcola l'ultima domenica di marzo
    int lastSunday = 31 - dayOfWeek;
    if (day > lastSunday || (day == lastSunday && hour >= 2)) {
      return true;  // Ora legale
    }
  }
  // Controlla se siamo in ottobre
  else if (month == 10) {
    // Calcola l'ultima domenica di ottobre
    int lastSunday = 31 - dayOfWeek;
    if (day < lastSunday || (day == lastSunday && hour < 3)) {
      return true;  // Ora legale
    }
  }
  // Tra aprile e settembre è sempre ora legale
  else if (month > 3 && month < 10) {
    return true;  // Ora legale
  }

  return false;  // Ora solare
  */


}



/*
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wakeup_reason();
*/

/*

void wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : log_i("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : log_i("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : log_i("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : log_i("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : log_i("Wakeup caused by ULP program"); break;

    default :
      // Configurazione del fuso orario
      configTime(0, 0, "pool.ntp.org", "time.nist.gov");
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        log_e("Impossibile ottenere l'orario");
        return;
      }
      log_i("rimposto l'orario causa mancata batteria");
      rtc.setTime(timeinfo.tm_sec, timeinfo.tm_min, timeinfo.tm_hour, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
      rtc.offset = 7200; // change offset value
      break;

  }
}
*/
