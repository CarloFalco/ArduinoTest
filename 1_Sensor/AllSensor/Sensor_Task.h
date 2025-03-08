#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_BME280.h>

#include <Adafruit_INA3221.h>
#include "MICS6814.h"
#include "SparkFunCCS811.h"
#include "Adafruit_PM25AQI.h"



#include "data_type_conversion.h"

#ifdef DEBUG_SENSOR
  #define print_Ses(x) if(Serial) Serial.print("Sensor: "); Serial.println(x)
#else
  #define print_Ses if(Serial) 
#endif


float matLinInt(float x1, float x2, float y1, float y2, float x0){
  return y1 + (x0 - x1) * (y2 - y1) / (x2 - x1);
}

// ANEMOMETRO



class Anemometer {
  public:
    Anemometer(int pin, float diameter);
    void begin();
    void kill();
    void update();
    
    float getWindSpeed_ms(void);
    float getWindSpeed_kph(void);
    String getIntensityString(void);
    
  private:
    int _pin;
    float _diameter;
    float _circumference;
    volatile int _rotationCount;
    int _arrayIndex;
    static const int _numElements = 5; // Numero di elementi per la media mobile
    int _rotationCounts[_numElements];

    float _windSpeed = 0;

    TaskHandle_t _taskHandle;

    static Anemometer* instance; // Puntatore statico all'istanza della classe

    static void countRevolutions();
    static void anemometerTask(void* pvParameters);
};

Anemometer* Anemometer::instance = nullptr; // Inizializza il puntatore statico

Anemometer::Anemometer(int pin, float diameter) 
  : _pin(pin), _diameter(diameter), _rotationCount(0), _arrayIndex(0), _circumference(M_PI*_diameter){
  for (int i = 0; i < _numElements; i++) {
    _rotationCounts[i] = 0;
  }
  ;
}

void Anemometer::begin() {
  pinMode(_pin, INPUT_PULLUP);
  instance = this; // Assegna l'istanza corrente al puntatore statico
  attachInterrupt(digitalPinToInterrupt(_pin), countRevolutions, FALLING);
  xTaskCreate(anemometerTask, "evaluation of anemometer task", 2048, this, 1, &_taskHandle);
}

void Anemometer::kill() {
  detachInterrupt(digitalPinToInterrupt(_pin)); 
  vTaskDelete(_taskHandle);
}

void Anemometer::update() {
  // Aggiorna l'array delle rotazioni
  _rotationCounts[_arrayIndex] = _rotationCount;
  _arrayIndex = (_arrayIndex + 1) % _numElements; // Incrementa l'indice e lo resetta a 0 dopo 3 cicli

  // Calcola la media mobile delle rotazioni
  float sum = 0;
  for (int i = 0; i < _numElements; i++) {
    sum += _rotationCounts[i];
  }
  float averageRotationCountSum = sum / _numElements;

  // Calcola la frequenza media di rotazione
  float frequency = averageRotationCountSum / (TASK_ANEMOMETER / 1000.0); // Rotazioni per secondo

  // Calcola la velocità del vento
  float _windSpeed = frequency * _circumference;

  print_Ses("Velocità del vento (media mobile): " + String(_windSpeed) + " [m/s]");
  print_Ses("Velocità del vento (media mobile): " + String(_windSpeed * 3.6) + " [km/h]");
  print_Ses("Velocità del vento (media mobile): " + getIntensityString());

  // Resetta il contatore di rotazioni
  _rotationCount = 0;
}

float Anemometer::getWindSpeed_ms(){
  return _windSpeed;
}

float Anemometer::getWindSpeed_kph(){
  return _windSpeed * 3.6;
}

String Anemometer::getIntensityString(void) {
  // FONTE : https://it.wikipedia.org/wiki/Scala_di_Beaufort
  float windSpeed_kph = _windSpeed * 3.6;
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



// Dichiarazione dell'oggetto Direzione del vento
class Windvane {
  public:
    Windvane(uint8_t addr);
    String getDirection(void);
    float getWindAngle(void);

  private:
    void readRawAngle(void);
    void rawToAngle(void);
    uint16_t _rawAngle;
    float _angle;
    uint8_t _addr;
};

Windvane::Windvane(uint8_t addr) 
  : _addr(addr), _rawAngle(0), _angle(0){
    // Costruttore vuoto
}

void Windvane::readRawAngle(void) {
  Wire.beginTransmission(_addr);
  Wire.write(RAW_ANGLE);
  Wire.endTransmission();
  Wire.requestFrom(_addr, 2);
  _rawAngle = Wire.read() << 8 | Wire.read();
  rawToAngle();

}

void Windvane::rawToAngle(void) {
  _angle = (_rawAngle * 360.0) / 4096.0; // Converti il valore grezzo in gradi
}

String Windvane::getDirection(void) {
  readRawAngle();
  float angle = _angle;
  if (angle >= 337.5 || angle < 22.5) return "N";
  else if (angle >= 22.5 && angle < 67.5) return "NE";
  else if (angle >= 67.5 && angle < 112.5) return "E";
  else if (angle >= 112.5 && angle < 157.5) return "SE";
  else if (angle >= 157.5 && angle < 202.5) return "S";
  else if (angle >= 202.5 && angle < 247.5) return "SW";
  else if (angle >= 247.5 && angle < 292.5) return "W";
  else return "NW";
}

float Windvane::getWindAngle(void){
  readRawAngle();
  return _angle; 
}


// Dichiarazione dell'oggetto Pluviometro
class Raingauge {
  public:
    Raingauge(void);
    void countTray();
    void update();   
    int getLevel(); 

  private:
    int _rainDroppCount;
    float _rainDropp;
};

Raingauge::Raingauge(void) 
  : _rainDroppCount(0), _rainDropp(0){
}

void Raingauge::update() {

  float iterationHour = _rainDroppCount *  3600 / SLEEP_TIME;
  _rainDropp = iterationHour * CONVERSIONE_PLUVIOMETRO;
  print_Ses("rainDropp: " + String(_rainDropp) + "mm");

  _rainDroppCount = 0;

}

void Raingauge::countTray() {
  _rainDroppCount++;
}

int Raingauge::getLevel(){
  return (int)_rainDropp;
}

// Dichiarazione del oggetto Energy Sensor



// Estendiamo la classe Adafruit_INA3221
class INA3211 : public Adafruit_INA3221 {
  public:
    // Costruttore che chiama quello della classe base
    INA3211() : Adafruit_INA3221() {}
    //INA3211(uint8_t addr) : Adafruit_INA3221() {
    //  _addr = addr;
    // };
    
    // Metodo aggiuntivo: Calcola la potenza (V * I)
    float getPower(uint8_t channel);
    int vbToSoc(float Vmeas);
    
  private: 
    typedef struct {
      float current[3];
      float voltage[3];
      float power[3];
      int soc;
    } INA;

};

float INA3211 :: getPower(uint8_t channel) {
  float voltage = getBusVoltage(channel);  // Legge la tensione
  float current = getCurrentAmps(channel) ;  // Converte da mA a A
  return voltage * current;  // Restituisce la potenza in Watt
}


int INA3211::vbToSoc(float Vmeas){
  int Vbat[] = {4187,4111,4080,4047,3996,3940,3891,3849,3806,3759,3714,3670,3632,3596,3554,3501,3456,3411,3309,3150,2925};
  int DOD[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
  int n = sizeof(Vbat) / sizeof(Vbat[0]); // Numero di punti nel vettore.

  // Cerca l'intervallo in cui si trova x0.

  if (Vmeas>Vbat[0]) {
      return (int)100;
      }
  if (Vmeas<Vbat[n-1]) {
      return (int)0;
      }
       
  for (int i = 0; i < n - 1; i++) {
      if (Vbat[i] >= Vmeas && Vmeas > Vbat[i + 1]) {        
          return (int)(100 - matLinInt(Vbat[i], Vbat[i + 1], DOD[i], DOD[i + 1], Vmeas));
      }
  }
  return (int)0;

}



