/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "HttpReleaseUpdate.h"
#include "ESP32GithubOtaUpdate.h"
#include "secret.h"


const char* OTA_FILE_LOCATION = "https://raw.githubusercontent.com/CarloFalco/ArduinoTest/refs/heads/Mqtt/11_OTA/ESP_OTA/firmware.bin";
const char* VERSION_URL = "https://raw.githubusercontent.com/CarloFalco/ArduinoTest/refs/heads/Mqtt/11_OTA/ESP_OTA/version.txt";

const int current_fw_version = 2024120902;  // YYYYMMDDRR where R = release of the day

ESP32GithubOtaUpdate otaUpdate;

#define LED_BUILTIN 97

bool ledState = false; // Variabile per lo stato del LED

void setupWiFi() {
  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
    Serial.print("ESP8266");
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
    Serial.print("ESP32");
  #endif

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void setupOtaUpdate() {
  otaUpdate.setOTADownloadUrl(OTA_FILE_LOCATION);
  otaUpdate.setVersionCheckUrl(VERSION_URL);
  otaUpdate.setCurrentFirmwareVersion(current_fw_version);
  otaUpdate.setUpdateCheckInterval(60); // Check every 60 seconds.
  otaUpdate.begin();
}


void setup() {
  Serial.begin(115200);
  Serial.print("WelcomeToNewBuild");
  setupWiFi();   
  pinMode(LED_BUILTIN, OUTPUT);
  setupOtaUpdate();
}



void loop(){

  ledState = !ledState; // Inverti lo stato
  digitalWrite(LED_BUILTIN, ledState); // Imposta il nuovo stato

  delay(500);
}
