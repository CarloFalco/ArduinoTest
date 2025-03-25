#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "letsEncryptCaCrt.h"

#include "secret.h"


WiFiClientSecure espClient;

const char* host = "api.openweathermap.org";
const int httpsPort = 443;




#define MAX_JSON_SIZE 4095
char json_data_string[MAX_JSON_SIZE];
String json;
JsonDocument doc;




void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("ip: ");Serial.println(WiFi.localIP());

  Serial.println("Connesso!");

  delay(2000);


  espClient.setCACert(leCaCrtOWM);


 Serial.print("Connessione al server...");
  if (!espClient.connect(host, httpsPort)) {
    Serial.println("Connessione fallita!");
    return;
  }else{
    Serial.println("Connessione Riuscita");
  }



  String addr = "https://api.openweathermap.org/data/2.5/weather?q="+city_name+"&appid="+owkey;
  Serial.println(addr);  

  espClient.print(String("GET ") + addr + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Aspetta la risposta
  while (espClient.connected()) {
    String line = espClient.readStringUntil('\n');
    if (line == "\r") break; // Fine dell'header HTTP
  }

  // Leggi il corpo della risposta
  String response = "";
  while (espClient.available()) {
    response += espClient.readString();
  }

  Serial.println("Risposta ricevuta:");
  Serial.println(response);

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, response);
 // Estrai le coordinate (coord) dal JSON
  JsonObject coord = doc["coord"];
  String lat = String(coord["lat"]);
  String lon = String(coord["lon"]);


  String addr1 = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon+"&appid="+owkey;
  Serial.println(addr1);  

  String addr2 = "http://api.openweathermap.org/data/2.5/forecast?lat="+lat+"&lon="+lon+"&appid="+owkey;
  Serial.println(addr2);  
/*
  


  json = getRequest(addr.c_str());

  Serial.println(json);
  Serial.println("");

  deserializeJson(doc, json);
  if(json.containsKey("coord")){
    Serial.print("Hello");
  }
  //Serial.println(doc["coord"][0]);
  //Serial.println(doc["coord"][1]);
*/

/*

  String addr = "http://api.openweathermap.org/data/2.5/forecast?lat="+lat+"&lon="+lon+"&appid="+owkey;
  // String addr = "http://api.openweathermap.org/data/2.5/weather?lat="+lat+"&lon="+lon+"&appid="+owkey;

  Serial.println(addr);

  doc = getRequest(addr.c_str());
  Serial.println(doc);
  // JSONVar obj = JSON.parse(jsonBuffer);
*/



}

// temperature_values = [entry["main"]["temp"] for entry in data["list"]]

void loop() { 

}


/*

String getRequest(const char* url) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, url);  

  int code = http.GET();
  String payload = "{}";
  if (code > 0) {
    Serial.print("HTTP CODE: ");Serial.println(code);               
    payload = http.getString(); 
  } else {
    Serial.print("HTTP ERROR: ");Serial.println(code);
  }
  http.end();
  return payload;


}
*/


