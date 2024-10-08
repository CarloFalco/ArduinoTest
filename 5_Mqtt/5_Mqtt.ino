// TODO: inibire il risveglio dal pin di interrupt se si risveglia piu di 3 volte nello stesso periodo di powerlatch

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFiClientSecure.h"
#include "letsEncryptCaCrt.h"

#include "secret.h"


#define S_TO_uS_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define S_TO_mS_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */
#define mS_TO_uS_FACTOR 1000ULL  /* Conversion factor for milli seconds to micro seconds */




#define LED_BUILTIN 97

// WebServer server(80);

WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

/*
char  mqtt_server[100] = MQTTServer;
char  mqtt_client_id[50] = MQTTClientId;
char  mqtt_user[50] = MQTTUser;
char  mqtt_pass[50] = MQTTPass;

*/


uint8_t failedMqttSends = 0;
int count = 0;


void setup() {
  // pin configuration

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }


  Serial.print("ip: ");Serial.println(WiFi.localIP());

  espClient.setCACert(leCaCrt);
  mqtt_client.setServer(mqtt_server,8883);

  if (mqtt_client.connect(mqtt_client_id,mqtt_user,mqtt_pass)){
    Serial.println("MQTT connected");
  }else {
    Serial.print("MQTT connection failed, rc=");
    Serial.println(mqtt_client.state());
  }



}


void loop() {
  // Questo codice non verrà eseguito, poiché l'ESP32 si risveglia dal deep sleep e ricomincia da setup().

  count ++;
  char testString[100];
  sprintf(testString, "{\"msg\": \"helloWorld\". \"cycle_no\": %d}", count);
  Serial.println(testString);

  if(!mqtt_client.publish("TESTWS",(const char * ) testString, 1)){
    failedMqttSends++;
  }
  else{
    failedMqttSends = 0;
    Serial.println("Messagge send Correctly");
  }
  delay(5000);

} 
