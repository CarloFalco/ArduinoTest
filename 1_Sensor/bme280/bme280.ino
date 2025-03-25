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
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // use I2C interface

#define PIN_5V 4
#define PIN_3V 11

typedef struct {
  float BME280[3];    // Flag di abilitazione notifiche utenti [1/0] abilitato/non abilitato
} Sensor;

Sensor sensor;


void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 Sensor event test"));

  // --------- SETUP BME280 ---------- //
  // --------- SETUP BME280 ---------- //

  pinMode(PIN_3V, OUTPUT);
  digitalWrite(PIN_3V, HIGH);  // turn the 3.3V
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  // turn the 5V



  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
}

void loop() {

  readBME280();
  Serial.print(F("Temperature = "));
  Serial.print(sensor.BME280[0]);
  Serial.println(" *C");

  Serial.print(F("Humidity = "));
  Serial.print(sensor.BME280[1]);
  Serial.println(" %");

  Serial.print(F("Pressure = "));
  Serial.print(sensor.BME280[2]);
  Serial.println(" hPa");

  Serial.println();
  delay(1000);
}


void readBME280(void){
  sensor.BME280[0] = bme.readTemperature();
  sensor.BME280[1] = bme.readHumidity();
  sensor.BME280[2] = bme.readPressure() / 100.0F; // Convert to hPa
}


