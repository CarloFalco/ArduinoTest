#include <Wire.h>
#include <SPI.h>



#include "sensor_task.h"
#include "data_type_conversion.h"


#define LED_BUILTIN 97


// DEFINIZIONE DEL ANEMOMETRO
Anemometer anemometer(ANEMOMETER_PIN, ANEMOMETER_DIAMETER);
Windvane windvane(AS5600_ADDRESS);
Raingauge raingauge;
Adafruit_BME280 bme; 
MICS6814 gasSensor;
CCS811 co2Sensor(CCS811_ADDRESS);
INA3211 ina3221;



void setup() {
  Wire.begin();
  
  Serial.begin(115200);
  delay(500);
  Serial.println("Hello");

  pinMode(LED_BUILTIN, OUTPUT);
  neopixelWrite(LED_BUILTIN,0,0,0);
  
  pinMode(PSW_5V, OUTPUT);
  digitalWrite(PSW_5V, HIGH);  // turn the 5V

  pinMode(PSW_3V3, OUTPUT);
  digitalWrite(PSW_3V3, HIGH);  // turn the 3.3V


  delay(500);

  // inizializzazione del anemometro e relativo tas  
  
  // --------- SETUP anemometer ---------- //
  anemometer.begin();
  // --------- SETUP raingauge ---------- //
  raingauge.update();
  // --------- SETUP BME280 ---------- //
  bme.begin(BME280_ADDRESS);
  // --------- SETUP mics6814 ---------- //
  gasSensor.begin();
  // --------- SETUP CCS811 ---------- //
  co2Sensor.begin();
  // --------- SETUP INA3221 ---------- //
  ina3221.begin(INA_ADDRESS);




  ina3221.setAveragingMode(INA3221_AVG_16_SAMPLES);

  // Set shunt resistances for all channels to 0.05 ohms
  for (uint8_t i = 0; i < 3; i++) {
    ina3221.setShuntResistance(i, 0.05);
  }
  // limits:
  ina3221.setPowerValidLimits(3.0 /* lower limit */, 15.0 /* upper limit */);






  neopixelWrite(LED_BUILTIN,64,0,0);
  delay(1500);


  // chiudo il task relativo al anemometro
  anemometer.kill();
  co2Sensor.readAlgorithmResults();

  delay(1500);
  neopixelWrite(LED_BUILTIN,0,64,0);
  printSensor();
  delay(1500);
}

void loop() {

  co2Sensor.readAlgorithmResults();

  printSensor();
  delay(3000);


}







void printSensor(void){
  Serial.println("-------------------------------");
  Serial.println("Velocità del vento (media mobile): " + String(anemometer.getWindSpeed_ms()) + " [m/s]");
  Serial.println("Velocità del vento (media mobile): " + String(anemometer.getWindSpeed_kph()) + " [km/h]");
  Serial.println("Velocità del vento (media mobile): " + anemometer.getIntensityString());
  Serial.println("");  
  Serial.println("Direzione del vento: " + windvane.getDirection() + " []");
  Serial.println("Velocità del vento: " + String(windvane.getWindAngle()) + " [*]");
  Serial.println("");
  Serial.println("Precipitazione: " + String(raingauge.getLevel()) + " [mm]");
  Serial.println("");
  Serial.println("Temperature = " + String(bme.readTemperature()) + " [*C]");
  Serial.println("Humidity = " + String(bme.readHumidity()) + " [%]");
  Serial.println("Pressure = " + String(bme.readPressure()) + " [hPa]");

  Serial.println("");
  Serial.println("Carbon Monoxide = " + String(gasSensor.getCO()) + " [ppm]");
  Serial.println("Nitrogen Dioxide = " + String(gasSensor.getNO2()) + " [ppm]");
  Serial.println("Ammonia = " + String(gasSensor.getNH3()) + " [ppm]");
  Serial.println("");
  Serial.println("Carbon Dioxide = " + String(co2Sensor.getCO2()) + " [ppm]");
  Serial.println("Nitrogen Dioxide = " + String(co2Sensor.getTVOC()) + " [ppm]");
  
  Serial.println("");

  for (uint8_t i = 0; i < 3; i++) {
    float voltage = ina3221.getBusVoltage(i);
    float current = ina3221.getCurrentAmps(i) * 1000; // Convert to mA
    Serial.print("Channel " + String(i) + ": ");
    Serial.print("Voltage = " + String(voltage) + "[V] ");
    Serial.println("Current = " + String(current) + " [mA]");
  }
  Serial.println("getPower: " + String(ina3221.getPower(2)));
  Serial.println("SOC = " + String(ina3221.vbToSoc(ina3221.getBusVoltage(1)*1000)) + " [ %]");

}


