//TODO:  controllare e impostare l'invalidita del tempo in caso il tempo sia piu vecchio del 2024

#include <WiFi.h>
#include <ESP32Time.h>
#include "secret.h"

ESP32Time rtc(3600);  // offset in seconds GMT+1ù



#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

void setup_rtc_time(ESP32Time *);
void check_DST(ESP32Time *);

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("ip: ");Serial.println(WiFi.localIP());

  // setup time
  setup_rtc_time(&rtc);


}

void loop() {
  String dataTime = rtc.getTime("%A, %B %d %Y %H:%M:%S");
  log_i("%s", dataTime.c_str());
  delay(1000);

  check_DST(&rtc);

}


void setup_rtc_time(ESP32Time *rtc){
    // Configurazione del fuso orario
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        if (!getLocalTime(&timeinfo)){
            log_e("Impossibile ottenere l'orario");
            return;
        }
    }

    rtc->setTime(timeinfo.tm_sec, timeinfo.tm_min, timeinfo.tm_hour+1, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

    check_DST(rtc);

}



void check_DST(ESP32Time *rtc) {
  // Regole per l'ora legale in Europa
  static bool firstTimeHere = true;
  static bool SolarTime = true; // variabile che indica se sono con l'orario solare o legale

  // L'ora legale inizia l'ultima domenica di marzo alle 2:00 AM e termina l'ultima domenica di ottobre alle 3:00 AM
  tm dt = rtc->getTimeStruct();

  int month = dt.tm_mon + 1;
  int day = dt.tm_mday;
  int hour = dt.tm_hour;
  int dayOfWeek = dt.tm_wday;
/*
  Serial.println("Month: " + String(month));   // (String) returns time with specified format
  Serial.println("day: " + String(day));   // (String) returns time with specified format
  Serial.println("hour: " + String(hour));   // (String) returns time with specified format
  Serial.println("dayOfWeek: " + String(dayOfWeek));   // (String) returns time with specified format
*/


  if (month == 3) {
    bool lastSunday = (31 - day)< 7;
    if ((lastSunday && dayOfWeek == 0 && hour >= 2)  && (SolarTime == false || firstTimeHere == true)) {
      // imposto l'ora legale facendo il tempo attuale piu 1h in millisecond
      
      // Serial.println("epoch time: " + String(rtc->getEpoch())); // (String) returns time
      rtc->setTime(rtc->getEpoch()+(60*60)); 
      // variabile che indica se sono con l'orario solare o legale
      SolarTime = true; 
      firstTimeHere = false;
    }
  } else if (month == 10) {
    // Calcola l'ultima domenica di ottobre
    bool lastSunday = (31 - day)< 7;
    // Serial.println("lastSunday: " + String(lastSunday));   // (String) returns time with specified format
    
    if ( (lastSunday && dayOfWeek == 0 && hour >= 3)  && (SolarTime == true || firstTimeHere == true)) {
      // imposto l'ora legale facendo il tempo attuale piu 1h in millisecond
      rtc->setTime(rtc->getEpoch()-(60*60)); 
      // variabile che indica se sono con l'orario solare o legale
      SolarTime = false; 
      firstTimeHere = false;
    }
  }




}
















