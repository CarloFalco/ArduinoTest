// CO
#include "MICS6814.h"
#include <driver/adc.h>


// Definizione dei pin dei sensori
const int pinNO2 = A3;
const int pinNH3 = A4;
const int pinCO = A5;



MICS6814 gasSensor;
int analogValue = 0;


#define LED_BUILTIN 97

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);  // turn the 3.3V
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the 3.3V


  Serial.begin(115200);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  gasSensor.begin();

}

// the loop function runs over and over again forever
void loop() {


  //valueNH3 = map(analogRead(pinNH3), 0, 4095.0, 1, 500);
  //valueNO2 = (map(analogRead(pinNO2), 0, 4095.0, 1, 1000))/100.0;
  //valueCO = map(analogRead(pinCO), 0, 4095.0, 1, 1000);
  float valueCO = gasSensor.getCO();
  float valueNO2 = gasSensor.getNO2();
  float valueNH3 = gasSensor.getNH3();



  Serial.print("NH3:");
  Serial.print(valueNH3);
  Serial.print(", ");
  Serial.print("NO2:");
  Serial.print(valueNO2);
  Serial.print(", ");
  Serial.print("CO:");
  Serial.println(valueCO);



  //digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //delay(1000);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
  Serial.println("");
}


int CO_ppm(double rawValue){
    
    Serial.println(rawValue);
    double ppm = 3.027*exp(1.0698*(rawValue*3.3/4095));
    Serial.println(ppm);
    return ppm;
}
