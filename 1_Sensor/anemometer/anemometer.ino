#define LED_BUILTIN 48
#define FROM_mm_to_m 0.001
#define FROM_ms_to_s 0.001

const float pi = 3.14159;
const int numElements = 5; // Numero di elementi per la media mobile

#define TASK_ANEMOMETER 200  // Periodo del task in millisecondi

class Anemometer {
  public:
    Anemometer(int pin, float diameter);
    void begin();
    void update();
    String getDirection(float windSpeed_kph);
    
  private:
    int _pin;
    float _diameter;
    volatile int _rotationCount;
    int _rotationCounts[numElements];
    int _arrayIndex;
    TaskHandle_t _taskHandle;

    static Anemometer* instance; // Puntatore statico all'istanza della classe

    static void countRevolutions();
    static void anemometerTask(void* pvParameters);
};

Anemometer* Anemometer::instance = nullptr; // Inizializza il puntatore statico

Anemometer::Anemometer(int pin, float diameter) 
  : _pin(pin), _diameter(diameter), _rotationCount(0), _arrayIndex(0) {
  for (int i = 0; i < numElements; i++) {
    _rotationCounts[i] = 0;
  }
}

void Anemometer::begin() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(_pin, INPUT_PULLUP);
  instance = this; // Assegna l'istanza corrente al puntatore statico
  attachInterrupt(digitalPinToInterrupt(_pin), countRevolutions, FALLING);

  xTaskCreate(anemometerTask, "evaluation of anemometer task", 2048, this, 1, &_taskHandle);
}

void Anemometer::update() {
  // Aggiorna l'array delle rotazioni
  _rotationCounts[_arrayIndex] = _rotationCount;
  Serial.print("rotationCount: " + String(_rotationCount) + "\n");
  _arrayIndex = (_arrayIndex + 1) % numElements; // Incrementa l'indice e lo resetta a 0 dopo 3 cicli

  // Calcola la media mobile delle rotazioni
  float sum = 0;
  for (int i = 0; i < numElements; i++) {
    sum += _rotationCounts[i];
  }
  float averageRotationCountSum = sum / numElements;

  Serial.print("Media1: " + String(averageRotationCountSum) + "\n");

  // Calcola la frequenza media di rotazione
  float frequency = averageRotationCountSum / (TASK_ANEMOMETER / 1000.0); // Rotazioni per secondo
  // Calcola la circonferenza
  float circumference = pi * _diameter;
  // Calcola la velocità del vento
  float windSpeed = frequency * circumference;

  Serial.print("Velocità del vento (media mobile): " + String(windSpeed) + " [m/s] \n");
  Serial.print("Velocità del vento (media mobile): " + String(windSpeed * 3.6) + " [km/h] \n");
  Serial.print("Velocità del vento (media mobile): " + getDirection(windSpeed * 3.6) + "\n");

  // Resetta il contatore di rotazioni
  _rotationCount = 0;
}

String Anemometer::getDirection(float windSpeed_kph) {
  // FONTE : https://it.wikipedia.org/wiki/Scala_di_Beaufort
  if (windSpeed_kph < 1) return "Calma";
  else if (windSpeed_kph >= 1 && windSpeed_kph < 6) return "Bava di vento";
  else if (windSpeed_kph >= 6 && windSpeed_kph < 11) return "Brezza leggera";
  else if (windSpeed_kph >= 11 && windSpeed_kph < 19) return "Brezza tesa";
  else if (windSpeed_kph >= 19 && windSpeed_kph < 29) return "Vento moderato";
  else if (windSpeed_kph >= 29 && windSpeed_kph < 39) return "Vento teso";
  else if (windSpeed_kph >= 39 && windSpeed_kph < 50) return "Vento fresco";
  else if (windSpeed_kph >= 50 && windSpeed_kph < 62) return "Vento forte";
  else if (windSpeed_kph >= 62 && windSpeed_kph < 75) return "Burrasca";
  else if (windSpeed_kph >= 75 && windSpeed_kph < 87) return "Burrasca forte";
  else if (windSpeed_kph >= 88 && windSpeed_kph < 102) return "Tempesta";
  else if (windSpeed_kph >= 102 && windSpeed_kph < 117) return "Tempesta violenta";
  else if (windSpeed_kph >= 117) return "Uragano";
  else return "Calma";
}

void Anemometer::countRevolutions() {
  instance->_rotationCount++;
}

void Anemometer::anemometerTask(void* pvParameters) {
  Anemometer* anemometer = static_cast<Anemometer*>(pvParameters);
  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(TASK_ANEMOMETER);

  while (true) {
    anemometer->update();
    vTaskDelayUntil(&lastWakeTime, period);
  }
}

// Dichiarazione dell'oggetto Anemometer
Anemometer anemometer(39, 80 * 2 * FROM_mm_to_m);

void setup() {
  Serial.begin(115200);
  

  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)


  anemometer.begin();
}

void loop() {
  // Il loop principale può rimanere vuoto o contenere altre logiche
}



