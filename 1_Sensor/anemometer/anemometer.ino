#define LED_BUILTIN 48
#define FROM_mm_to_m 0.001
#define FROM_ms_to_s 0.001


const float diameter = 80*FROM_mm_to_m*2; // Diametro del rotore in metri
const float pi = 3.14159;
const int numElements = 5; // Numero di elementi per la media mobile

unsigned long previousMillis = 0;
const long interval = 200; // Intervallo di tempo in millisecondi (1 secondo)

volatile int rotationCount = 0;
int rotationCounts[numElements] = {0, 0, 0, 0, 0}; // Array per memorizzare le ultime tre misurazioni di rotazioni
int ArrayIndex = 0;

const int AnemometerPin = 40; // Pin a cui è collegato il bottone

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.print("Diameter: ");
  Serial.print(diameter);
  Serial.println(" [m]");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)

  attachInterrupt(digitalPinToInterrupt(AnemometerPin), countRevolutions, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Aggiorna l'array delle rotazioni
    rotationCounts[ArrayIndex] = rotationCount;
    ArrayIndex = (ArrayIndex + 1) % numElements; // Incrementa l'indice e lo resetta a 0 dopo 3 cicli

    // Calcola la media mobile delle rotazioni
    float sum = 0;
    for (int i = 0; i < numElements; i++) {
      sum += rotationCounts[i];
    }


    // Calcola la media mobile delle rotazioni
    float averageRotationCount = (rotationCounts[0] + rotationCounts[1] + rotationCounts[2]) / 3.0;

    // Calcola la frequenza media di rotazione
    float frequency = averageRotationCount / (interval / 1000.0); // Rotazioni per secondo
    // Calcola la circonferenza
    float circumference = pi * diameter;
    // Calcola la velocità del vento
    float windSpeed = frequency * circumference;

    Serial.print("Velocità del vento (media mobile): ");
    Serial.print(windSpeed);
    Serial.println(" m/s");

    // Resetta il contatore di rotazioni
    rotationCount = 0;
  }
}


void countRevolutions() {
  rotationCount++;
}