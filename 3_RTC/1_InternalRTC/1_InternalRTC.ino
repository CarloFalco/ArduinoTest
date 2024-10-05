//TODO:  controllare e impostare l'invalidita del tempo in caso il tempo sia piu vecchio del 2024

#include <WiFi.h>
#include <ESP32Time.h>
#include "secret.h"
#include "Utilities.h"


#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

extern ESP32Time rtc(3600);  // Dichiarazione di rtc


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















