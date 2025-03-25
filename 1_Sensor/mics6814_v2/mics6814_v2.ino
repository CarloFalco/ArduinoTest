#include "MICS6814.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"

#define PIN_5V 4
#define PIN_3V 11

#define PIN_CO  ADC1_CHANNEL_4
#define PIN_NO2 ADC1_CHANNEL_5
#define PIN_NH3 ADC1_CHANNEL_6

static esp_adc_cal_characteristics_t adc1_value;


// nh3 1218, no2 3673, co 4095
// nh3 1147, no2 3724, co 4095
// nh3 482, no2 3262, co 4095
// NH3 923, no2 268, CO 1023

MICS6814 gas(PIN_CO, PIN_NO2, PIN_NH3);

void setup() {

  pinMode(PIN_3V, OUTPUT);
  digitalWrite(PIN_3V, HIGH);  // turn the 3.3V
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  // turn the 5V


  Serial.begin(115200);



  Serial.println("MICS-6814 Sensor Sample");
  Serial.print("Calibrating Sensor");
  gas.begin(false);
  // gas.calibrate();+


}

void loop() {
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