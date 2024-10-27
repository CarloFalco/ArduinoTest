#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <EEPROM.h>
// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you

#include "secret.h"

// FLASH
#define FLASH_LED_PIN 4
// SERVOMOTORE
#define MOS_SERVO_PIN 2
#define FEED_MAX_COUNT 6
#define SERVO_PIN 13
#define SERVO_MOVE_ON 95
#define SERVO_MOVE_OFF 90
#define FEED_INIT_COUNT 2



typedef struct {
  String CurrentPhotoReqId;  // ID del utente che ha richiesto la foto
  String AuthId[4];          // ID degli utenti
  bool NotificationId[4];    // Flag di abilitazione notifiche utenti [1/0] abilitato/non abilitato

  bool stateIRpin;      // stato del sensore infrarossi
  bool stateSendPhoto;  // stato richiesta di foto
  bool stateFeedCat;    // stato richiesta feedcat
  bool stateFlash;      // stato flash
  int feedCount;        // quantità di cibo
  int eprom_good;       // eeprom non corrotta

} MyStruct;


MyStruct myStruct;

WiFiClientSecure clientTCP;

String BOTtoken = String(BOTtokenKey);
UniversalTelegramBot bot(BOTtoken, clientTCP);
   
//Checks for new messages every 1 second.
int botRequestDelay = 500;
unsigned long lastTimeBotRan;

bool savetoEEPROM = false;


void initWiFi_PwrOn(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  // Attesa della connessione
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione in corso...");
  }
  Serial.println("WiFi connected to " + String(ssid));
  Serial.println(WiFi.localIP());
}



// FUNZIONI RELATIVE ALLA EEPROM
void do_eprom_read() {
  EEPROM.begin(200);
  EEPROM.get(0, myStruct);
  if (myStruct.eprom_good == 11) {
    Serial.println("Good settings in the EPROM ");

    Serial.print("Notification: ");
    for (int i = 0; i < 4; i++) {
      Serial.print("\t");
      Serial.print(myStruct.NotificationId[i]);
    }
    Serial.println(".");

    Serial.println(myStruct.CurrentPhotoReqId);
    Serial.print("State IR: ");
    Serial.println(myStruct.stateIRpin);
    Serial.print("State sendphoto: ");
    Serial.println(myStruct.stateSendPhoto);
    Serial.print("State feedcat: ");    
    Serial.println(myStruct.stateFeedCat);
    Serial.print("State Flash: ");   
    Serial.println(myStruct.stateFlash);
    Serial.print("Count Num: ");
    Serial.println(myStruct.feedCount);

  } else {
    Serial.println("EPROM canot be read ");
    myStruct = {
      String(""),
      { String(CHAT_ID_1), String(""), String(""), String("") },
      { 1, 0, 0, 0 },
      false,                //stateIRpin
      false,                //stateSendPhoto
      false,                //stateFeedCat
      LOW,                  // stato flash
      FEED_INIT_COUNT,      // tempo di feed
      11                    // stato EEPROM
    };
    do_eprom_write();
  }
}

void do_eprom_write() {
  Serial.println("Writing to EPROM ...");
  EEPROM.begin(200);
  EEPROM.put(0, myStruct);
  EEPROM.commit();
  EEPROM.end();
}


// FUNZIONI RELATIVE AL BOT
void handleNewMessages(int numNewMessages) {

  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);


    // verifico che sia un utente autorizzato
    int checkIdAuth = 0;
    int j = 0;
    for (j = 0; j < sizeof(myStruct.AuthId); j++) {  // TODO
      if (chat_id == myStruct.AuthId[j]) {
        checkIdAuth = 1;
        break;
      }
    }
    if (!checkIdAuth) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;



    if (text == "/start" || text == "/back") {
      String welcome = "Welcome to the ESP32-CAM Telegram bot.\n";
      welcome += "/photo : takes a new photo\n";
      welcome += "/flash : toggle flash LED\n";
      welcome += "/feed : feed the cat\n";
      welcome += "/settings : possibily to modify basic setting\n";
      welcome += "You'll receive a photo whenever motion is detected.\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/settings") {  // TODO: IMPLEMENTARE LE FUNZIONALITA
      String welcome = "In this menu, you can change information about the dispenser.\n";
      welcome = "/notification, here you can change notification settings.\n";
      welcome += "/setfeed : Change food quantity\n";
      welcome += "/back : back to start menu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/setfeed") {  // Funzionalità legate alle feeder
      String welcome = "/setfeed, here you can increase or decrease food quantity.\n";
      welcome += "/increase : Increase food quantity\n";
      welcome += "/decrease : Decrease food quantity\n";
      welcome += "/back : back to start menu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/increase") {
      if (myStruct.feedCount < FEED_MAX_COUNT) {
        // vado a modificare il valore
        myStruct.feedCount++;

        Serial.print("feedCount: ");
        Serial.println(myStruct.feedCount);
        // vado a salvare il valore in eeprom
        // savetoEEPROM = true;
      } else {
        Serial.println("you reach the maximum value");
      }
    }

    if (text == "/decrease") {
      if (myStruct.feedCount > 1) {
        // vado a modificare il valore
        myStruct.feedCount--;
        Serial.print("feedCount: ");
        Serial.println(myStruct.feedCount);
        // vado a salvare il valore in eeprom
        // savetoEEPROM = true;
      } else {
        Serial.println("you reach the minimum value");
      }
    }


    if (text == "/notification") {  // Funzionalità legate alle notifiche
      String welcome = "/notification, here you can change notification settings.\n";
      welcome += "/dontshutup : enable motion sensor notification\n";
      welcome += "/shutup : disalbe motion sensor notification\n";
      welcome += "/back : back to start menu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/shutup") {
      // controllo se il valore precedente è differente da quello che ho chiesto
      if (myStruct.NotificationId[j] == true) {
        // vado a modificare il valore
        myStruct.NotificationId[j] = false;
        // vado a salvare il valore in eeprom
        // savetoEEPROM = true;

        Serial.print("Notification: ");
        for (int i = 0; i < 4; i++) {
          Serial.print("\t");
          Serial.print(myStruct.NotificationId[i]);
        }
        Serial.println(".");

      }
      bot.sendMessage(chat_id, "OK, I will disable notifications.\n", "Markdown");
    }
    if (text == "/dontshutup") {
      // controllo se il valore precedente è differente da quello che ho chiesto
      if (myStruct.NotificationId[j] == false) {
        // vado a modificare il valore
        myStruct.NotificationId[j] = true;
        // vado a salvare il valore in eeprom
        // savetoEEPROM = true;

        Serial.print("Notification: ");
        for (int i = 0; i < 4; i++) {
          Serial.print("\t");
          Serial.print(myStruct.NotificationId[i]);
        }
        Serial.println(".");
      }

      bot.sendMessage(chat_id, "OK, I will re-activate notifications.\n", "Markdown");
    }
  }
}


void setup() {

  // Init Serial Monitor
  Serial.begin(115200);

  // Inizializzo la mia struttura
  do_eprom_read();

  // Connect to Wi-Fi
  initWiFi_PwrOn(NETWORK_SSID, PASSWORD);

  // Crea i task
  Scheduler_PwrOn();
}

void task1(void* pvParameters) {  // 60 secondo
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(60000);
  while (true) {
    // Esegui il codice del task 1
    //Serial.println("Task 1");
    if (savetoEEPROM) {
      savetoEEPROM = false;
      do_eprom_write();
    }
    vTaskDelayUntil(&lastWakeTime, period);
  }
}


void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

}



// CONDIZIONI INIZIALI//
void Scheduler_PwrOn(void) {
  xTaskCreatePinnedToCore(
    task1,         // Task function.
    "Task1",       // name of task.
    2048,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &task1Handle,  // Task handle to keep track of created task
    1);            // pin task to core 0
           // pin task to core 1

}
