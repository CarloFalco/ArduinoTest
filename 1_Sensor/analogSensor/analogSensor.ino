/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/
#include <driver/adc.h>

#define BIT_NUM = 4096.0

int ledPin = 13;      // select the pin for the LED
int BitsensorValueN = 0;  // variable to store the value coming from the sensor
float BitsensorValue = 0;  // variable to store the value coming from the sensor
float Voltsensor1 = 0;  // Voltage on the sensor
float Voltsensor2 = 0;  // Voltage on the pin



void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);   
  
  
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11); 
}

void loop() {
  // read the value from the sensor:

  for (int i = 1; i<=15; i ++){
    BitsensorValueN = BitsensorValueN + adc1_get_raw(ADC1_CHANNEL_3);
    delayMicroseconds(100);
  } 


  BitsensorValue = BitsensorValueN/15;


  Voltsensor2 = map(BitsensorValue, 0, BIT_NUM, 0, 3300)/1000.0;




  Serial.print(BitsensorValue); 
  Serial.print(" [bit]  |   ");
  
  Serial.print(Voltsensor2); 
  Serial.print(" [V]  |   ");

  delay(500);
  BitsensorValueN = 0;
} 
