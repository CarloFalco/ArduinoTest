#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFiClientSecure.h"
#include "letsEncryptCaCrt.h"

// #include "calibration.h"

#include "secret.h"


#define MAX_JSON_SIZE 4095


typedef struct {
    String nome;
    int valore;
} Variabile;

Variabile variabili[3] = {
    {"variabile1", 0},
    {"variabile2", 0},
    {"variabile3", 0}
};

TaskHandle_t task1Handle;
TaskHandle_t task2Handle;

#define S_TO_uS_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define S_TO_mS_FACTOR 1000ULL  /* Conversion factor for milli seconds to seconds */
#define mS_TO_uS_FACTOR 1000ULL  /* Conversion factor for milli seconds to micro seconds */


#define LED_BUILTIN 97
// WebServer server(80);

WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);
int count = 0;


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

  // Gestione dei diversi topic
  if (strcmp(topic, "paperino") == 0) {
    // Gestisci il messaggio del topic "paperino"
    Serial.println("Messaggio dal topic paperino");
    // Aggiorna le variabili in base ai dati del JSON
    for (int i = 0; i < 3; i++) {
      if (doc.containsKey(variabili[i].nome)) {
        variabili[i].valore = doc[variabili[i].nome].as<int>();
      }
    }

    char json_data_string[MAX_JSON_SIZE];  
    to_json(json_data_string);

    mqtt_client.publish("CalibVal", (const char *) json_data_string, 1);

    // Qui puoi aggiungere il codice per gestire i dati specifici del topic "paperino"
  } else if (strcmp(topic, "pluto") == 0) {
    // Gestisci il messaggio del topic "pluto"
    Serial.println("Messaggio dal topic pluto");
    // Qui puoi aggiungere il codice per gestire i dati specifici del topic "pluto"
  } else {
    Serial.println("Topic sconosciuto");
  }


/*
  const char* msg = doc["msg"];
  String ledStatusString = doc["LedSts"];
  String provaString = doc["prova"];
  int ledStatus = ledStatusString.toInt();
  Serial.print("Messaggio: ");   Serial.println(msg);

  Serial.print("Stato LED: ");   Serial.println(ledStatus);

  Serial.print("String: ");  Serial.print(provaString);   Serial.print("\t ");   Serial.println(provaString.toInt());

*/


}


void to_json(char * json){

DynamicJsonDocument doc(MAX_JSON_SIZE);

// JsonObject epoc = doc["epoc"].to<JsonObject>();

doc["variabile1"] = String(variabili[0].valore);
doc["variabile2"] = String(variabili[1].valore);
doc["variabile3"] = String(variabili[2].valore);
// doc.shrinkToFit();  // optional

serializeJson(doc, json, MAX_JSON_SIZE);

serializeJson(doc, Serial);

}


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


  mqtt_PwrOn();

  pinMode(LED_BUILTIN, OUTPUT);

  // Crea i task
  Scheduler_PwrOn();
}

void loop() {
    //Serial.print("Valore di variabile1: ");
    //Serial.println(variabili[0].valore);
    mqtt_client.loop();

    delay(100);
    
}






void mqtt_PwrOn(void){

  // Imposta la funzione di callback per i messaggi ricevuti
  mqtt_client.setCallback(mqttCallback);

  espClient.setCACert(leCaCrt);
  mqtt_client.setServer(mqtt_server,8883);

  if (mqtt_client.connect(mqtt_client_id,mqtt_user,mqtt_pass)){
    Serial.println("MQTT connected");
    mqtt_client.subscribe("paperino");
    mqtt_client.subscribe("pluto");

  }else {
    Serial.print("MQTT connection failed, rc=");
    Serial.println(mqtt_client.state());
  }


}



void task_blink(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000);

  while (true) {
    // Esegui il codice del task 1
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    count ++;
    char testString[100];
    sprintf(testString, "{\"msg\": \"helloWorld\". \"cycle_no\": %d}", count);
    Serial.println(testString);


    vTaskDelayUntil(&lastWakeTime, period);
  }
}




// Creazione dei task //
void Scheduler_PwrOn(void) {
/*
  #ifdef ENABLE_CALIBRATION
  xTaskCreatePinnedToCore(
    task_calib,         // Task function.
    "task_calib",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task1Handle,  // Task handle to keep track of created task
    0);            // pin task to core 0        // pin task to core 0    
  #endif
*/
  xTaskCreatePinnedToCore(
  task_blink,         // Task function.
  "task_blink",       // name of task.
  2048,          // Stack size of task
  NULL,          // parameter of the task
  1,             // priority of the task
  &task2Handle,  // Task handle to keep track of created task
  0);            // pin task to core 0        // pin task to core 0   
}



