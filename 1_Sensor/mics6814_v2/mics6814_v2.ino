#include "MICS6814.h"

#define PIN_CO  5
#define PIN_NO2 6
#define PIN_NH3 7

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

void setup() {
  Serial.begin(9600);

  Serial.println("MICS-6814 Sensor Sample");
  Serial.print("Calibrating Sensor");

  gas.calibrate();

  Serial.println("OK!");
}

void loop() {
  Serial.print("NH3: ");

  Serial.print(gas.measure(NH3));
  Serial.println("ppm");
  delay(50);
  float valueCO = gas.measure(CO);
  float valueNO2 = gas.measure(NO2);
  float valueNH3 = gas.measure(NH3);
  
  
  Serial.print("NH3:");
  Serial.print(valueNH3);
  Serial.print(", ");
  Serial.print("NO2:");
  Serial.print(valueNO2);
  Serial.print(", ");
  Serial.print("CO:");
  Serial.println(valueCO);
  
  
  

  delay(1000);
}