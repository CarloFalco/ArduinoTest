#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "secret.h"
#include "letsEncryptCaCrt.h"

// URL del server
//const char* serverUrl = "https://jsonplaceholder.typicode.com/posts/1"; // URL per il test
const char* serverUrl = "https://raw.githubusercontent.com/kakopappa/esp32-github-ota/main/version.txt";

void setup() {
  Serial.begin(115200);

  // Connessione alla rete Wi-Fi
  Serial.print("Connessione al Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnesso al Wi-Fi!");
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Test della richiesta HTTP GET
  testHttpGet();
}

void loop() {
  // Il loop è vuoto poiché la richiesta viene effettuata una volta nel setup
}

// Funzione per testare una richiesta HTTP GET
void testHttpGet() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure espClient;
    espClient.setCACert(leCaCrt_My);
    // client.setInsecure(); // Disabilita la verifica del certificato per test (non sicuro per produzione!)

    HTTPClient http;

    Serial.println("Inizializzo richiesta HTTP...");
    if (http.begin(espClient, serverUrl)) { // Imposta l'URL e il client sicuro
      Serial.println("Invio richiesta GET...");
      int httpCode = http.GET(); // Effettua la richiesta GET

      // Controllo del codice di stato HTTP
      if (httpCode > 0) {
        Serial.printf("Codice di stato HTTP: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) { // 200 OK
          String payload = http.getString();
          Serial.println("Risposta del server:");
          Serial.println(payload);
        }
      } else {
        Serial.printf("Errore nella richiesta GET: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end(); // Chiude la connessione HTTP
    } else {
      Serial.println("Impossibile inizializzare la connessione HTTP");
    }
  } else {
    Serial.println("Wi-Fi non connesso!");
  }
}
