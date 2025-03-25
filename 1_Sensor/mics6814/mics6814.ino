// CO
#include "MICS6814.h"
#include <driver/adc.h>




// nh3 1218, no2 3673, co 4095
// nh3 1147, no2 3724, co 4095
// nh3 1153, no2 3718, co 4095
// NH3 923, no2 268, CO 1023


MICS6814 gasSensor;
int analogValue = 0;

#define PIN_5V 4
#define PIN_3V 11


#define LED_BUILTIN 97

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(PIN_3V, OUTPUT);
  digitalWrite(PIN_3V, HIGH);  // turn the 3.3V
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  // turn the 5V


  Serial.begin(115200);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Initializzation: ");
  Serial.println(gasSensor.begin());


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

