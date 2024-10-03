#include "arduino.h"
#include "calibration.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


Variabile variabili[4] = {
    {"variabile1", 0},
    {"variabile2", 0},
    {"variabile3", 0},
    {"variabile4", 0}
};

void gestisciSeriale() {
    if (Serial.available() > 0) { // Controlla se ci sono dati disponibili
        String input = Serial.readStringUntil('\n'); // Legge l'input fino al carattere di nuova linea
        input.trim(); // Rimuove eventuali spazi bianchi

        if (input.startsWith("get ")) {
            String nome = input.substring(4); // Estrae il nome della variabile

            bool trovato = false;
            for (int i = 0; i < 4; i++) {
                if (variabili[i].nome == nome) {
                    Serial.print("Valore attuale di ");
                    Serial.print(nome);
                    Serial.print(": ");
                    Serial.println(variabili[i].valore);
                    trovato = true;
                    break;
                }
            }
            if (!trovato) {
                Serial.println("Nome della variabile non valido.");
            }
        } else if (input.startsWith("put ")) {
            int spazio = input.indexOf(' ', 4); // Trova la posizione dello spazio dopo 'put '
            String nome = input.substring(4, spazio); // Estrae il nome della variabile
            int valore = input.substring(spazio + 1).toInt(); // Estrae il valore da assegnare

            bool trovato = false;
            for (int i = 0; i < 4; i++) {
                if (variabili[i].nome == nome) {
                    variabili[i].valore = valore;
                    Serial.print("Variabile ");
                    Serial.print(nome);
                    Serial.print(" aggiornata a: ");
                    Serial.println(variabili[i].valore);
                    trovato = true;
                    break;
                }
            }
            if (!trovato) {
                Serial.println("Nome della variabile non valido.");
            }
        } else {
            Serial.println("Comando non valido. Usa 'put nome valore' o 'get nome'.");
        }
    }
}


void task_calib(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_CALIB);

  while (true) {
    // Esegui il codice del task 1
    gestisciSeriale();

    vTaskDelayUntil(&lastWakeTime, period);
  }
}
