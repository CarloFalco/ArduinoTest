#include <Wire.h>

#define LED_BUILTIN 97

// dati relativi alla direzione del vento
#define AS5600_ADDRESS 0x36
#define RAW_ANGLE 0x0C



// dati relativi al pluviometro
#define AREA_PLUVIOMETRO 63.25 //11.5 x 5.5 = 63.25 cm2
#define AREA_VASCHETTA 3 //3 ml ogni iterazione
#define CONVERSIONE_PLUVIOMETRO 0.4743 // 3 x 10^-3 /( 63.25 x 10^-6) = 3 x 10^3 / 63.25 = 47.43  [mm h oppure Litri per m2 ora]

#define CYCLE_RAIN_LENGTH 10 // tempo in secondi



int rainSensorPin = 40; // Pin a cui è collegato il sensore a effetto Hall
volatile int rainDroppCount = 0;



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  neopixelWrite(LED_BUILTIN,0,0,0);
  
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)

  
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Attendi che la porta seriale sia pronta
  Serial.println("AS5600 Magnet Position");

  // inizializzazione sensore pioggia
  attachInterrupt(digitalPinToInterrupt(rainSensorPin), countTray, FALLING);


}

void loop() {
  
  uint16_t rawAngle = readRawAngle();
  float angle = (rawAngle * 360.0) / 4096.0; // Converti il valore grezzo in gradi
  String direction = getDirection(angle);
  Serial.print("Angle: " + String(angle) + " degrees \t Direction: " + direction +  "\n");


  float NIterazioniOra = rainDroppCount *  3600 / CYCLE_RAIN_LENGTH;
  float LitriMetroOra = NIterazioniOra * CONVERSIONE_PLUVIOMETRO;
  Serial.print("LitriMetroOra: " + String(LitriMetroOra) + "mm \n");

  rainDroppCount = 0;
  delay(CYCLE_RAIN_LENGTH*1000); // Attende mezzo secondo prima di ripetere il ciclo
}


void countTray() {
  rainDroppCount++;
}


uint16_t readRawAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(RAW_ANGLE);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDRESS, 2);
  uint16_t rawAngle = Wire.read() << 8 | Wire.read();
  return rawAngle;
}

String getDirection(float angle) {
  if (angle >= 337.5 || angle < 22.5) return "N";
  else if (angle >= 22.5 && angle < 67.5) return "NE";
  else if (angle >= 67.5 && angle < 112.5) return "E";
  else if (angle >= 112.5 && angle < 157.5) return "SE";
  else if (angle >= 157.5 && angle < 202.5) return "S";
  else if (angle >= 202.5 && angle < 247.5) return "SW";
  else if (angle >= 247.5 && angle < 292.5) return "W";
  else return "NW";
}