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

// Funzione di callback per la ricezione dei messaggi MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Messaggio ricevuto su topic: ");
  Serial.println(topic);

  // Convertire il payload in una stringa JSON
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0'; // Aggiungi il terminatore di stringa

  Serial.print("Payload: ");
  Serial.println(message);

  // Parsing del JSON
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("Errore nel parsing del JSON: ");
    Serial.println(error.c_str());
    return;
  }

  const char* msg = doc["msg"];
  String ledStatusString = doc["LedSts"];
  String provaString = doc["prova"];
  int ledStatus = ledStatusString.toInt();
  Serial.print("Messaggio: ");
  Serial.println(msg);

  Serial.print("Stato LED: ");
  Serial.println(ledStatus);

  Serial.print("String: ");
  Serial.print(provaString);
  Serial.print("\t ");
  Serial.println(provaString.toInt());

  // Controllo del LED in base al messaggio ricevuto
  if (ledStatus == 1) {
    digitalWrite(LED_BUILTIN, HIGH); // Accendi LED
  } else {
    digitalWrite(LED_BUILTIN, LOW);  // Spegni LED
  }
}





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

  // Imposta la funzione di callback per i messaggi ricevuti
  mqtt_client.setCallback(mqttCallback);


  espClient.setCACert(leCaCrt);
  mqtt_client.setServer(mqtt_server,8883);

  if (mqtt_client.connect(mqtt_client_id,mqtt_user,mqtt_pass)){
    Serial.println("MQTT connected");
    
    if (mqtt_client.subscribe("paperino")) {
      Serial.println("Sottoscritto al topic 'paperino'");
    } else {
      Serial.println("Errore nella sottoscrizione al topic 'paperino'");
    }

  }else {
    Serial.print("MQTT connection failed, rc=");
    Serial.println(mqtt_client.state());
  }




}


void loop() {
  // Questo codice non verrà eseguito, poiché l'ESP32 si risveglia dal deep sleep e ricomincia da setup().
  mqtt_client.loop();

/*
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
  */
  delay(100);
} 
