#include <Wire.h>
#include "Adafruit_PM25AQI.h"


#define PIN_5V 4
#define PIN_3V 11



Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
PM25_AQI_Data pmSensor;


void setup() {
  pinMode(PIN_3V, OUTPUT);
  digitalWrite(PIN_3V, HIGH);  // turn the 3.3V
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  // turn the 5V
  pinMode(41, OUTPUT);
  digitalWrite(41, LOW);  // turn the 5V

  delay(300);
  Serial.begin(115200);
  
  Serial1.begin(9600, SERIAL_8N1, 17, 18);

  aqi.begin_UART(&Serial1);

}


void loop() {

  aqi.read(&pmSensor);

  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.print(F("PM 1.0: ")); Serial.print(pmSensor.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(pmSensor.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(pmSensor.pm100_standard);
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (environmental)"));
  Serial.print(F("PM 1.0: ")); Serial.print(pmSensor.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(pmSensor.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(pmSensor.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(pmSensor.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(pmSensor.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(pmSensor.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(pmSensor.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(pmSensor.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(pmSensor.particles_100um);
  Serial.println(F("---------------------------------------"));


  delay(5000); // Aspetta 5 secondi prima di ripetere la scansione
}