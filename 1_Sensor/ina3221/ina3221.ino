/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor
  This example shows how to take Sensor Events instead of direct readings
  
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2652

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include "Adafruit_INA3221.h"

// Create an INA3221 object
Adafruit_INA3221 ina3221;

// POWERLINE

int PSW_5V = 37;
int PSW_3V3 = 38;

float matLinInt(float x1, float x2, float y1, float y2, float x0){
    return y1 + (x0 - x1) * (y2 - y1) / (x2 - x1);
}


void setup() {
  Serial.begin(115200);


  pinMode(PSW_5V, OUTPUT);
  digitalWrite(PSW_5V,HIGH);

  pinMode(PSW_3V3, OUTPUT);
  digitalWrite(PSW_3V3,HIGH);

  //Serial.println("Temperature = " + String(ina3221.vbToSoc(ina3221.getVoltage(INA3221_CH2))) + " [*C]");


  // Initialize the INA3221
  if (!ina3221.begin(0x40, &Wire)) { // can use other I2C addresses or buses
    Serial.println("Failed to find INA3221 chip");
    while (1)
      delay(10);
  }

  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);

  // Set shunt resistances for all channels to 0.05 ohms
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 0.05);
  }
  // limits:
  ina3221.setPowerValidLimits(3.0 /* lower limit */, 15.0 /* upper limit */);
}

void loop() {

  // Display voltage and current (in mA) for all three channels
  for (uint8_t i = 0; i < 3; i++) {
    float voltage = ina3221.getBusVoltage(i);
    float current = ina3221.getCurrentAmps(i) * 1000; // Convert to mA

    Serial.print("Channel ");
    Serial.print(i);
    Serial.print(": Voltage = ");
    Serial.print(voltage, 3);
    Serial.print(" V, Current = ");
    Serial.print(current, 2);
    Serial.println(" mA");
  }
  Serial.print(" SOC = ");
  Serial.print(vbToSoc(ina3221.getBusVoltage(1)*1000));
  Serial.println(" %");
  Serial.println("");
  // Delay for 250ms before the next reading
  delay(250);

}


int vbToSoc(float Vmeas){
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
