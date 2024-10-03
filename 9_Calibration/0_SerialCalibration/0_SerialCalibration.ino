#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#define TASK_CALIB   100                // 0.1ms
#define ENABLE_CALIBRATION

#define LED_BUILTIN 97

typedef struct {
    String nome;
    int valore;
} Variabile;

Variabile variabili[4] = {
    {"variabile1", 0},
    {"variabile2", 0},
    {"variabile3", 0},
    {"variabile4", 0}
};

TaskHandle_t task1Handle;

void setup() {
    Serial.begin(115200); // Inizializza la comunicazione seriale a 9600 baud
    Serial.println("Usa 'put nome valore' per aggiornare una variabile o 'get nome' per chiedere il valore attuale:");

    pinMode(LED_BUILTIN, OUTPUT);

    // Crea i task
    Scheduler_PwrOn();
}

void loop() {
    log_e("Impossibile ottenere l'orario");
    delay(1000);
    // Il resto del tuo codice che utilizza le variabili
}



#ifdef ENABLE_CALIBRATION

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

#endif


void task_blink(void* pvParameters) {  
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000);

  while (true) {
    // Esegui il codice del task 1
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    vTaskDelayUntil(&lastWakeTime, period);
  }
}



// Creazione dei task //
void Scheduler_PwrOn(void) {

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

  xTaskCreatePinnedToCore(
  task_blink,         // Task function.
  "task_blink",       // name of task.
  2048,          // Stack size of task
  NULL,          // parameter of the task
  1,             // priority of the task
  &task1Handle,  // Task handle to keep track of created task
  0);            // pin task to core 0        // pin task to core 0   
}


