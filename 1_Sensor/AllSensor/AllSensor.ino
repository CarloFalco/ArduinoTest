#include <Wire.h>
#include <SPI.h>
#include "ArduinoJson.h"

#include "sensor_task.h"
#include "data_type_conversion.h"


#define LED_BUILTIN 97
#define MAX_JSON_SIZE 4095

// DEFINIZIONE DEL ANEMOMETRO
Anemometer anemometer(ANEMOMETER_PIN, ANEMOMETER_DIAMETER);
Windvane windvane(AS5600_ADDRESS);
Raingauge raingauge;
Adafruit_BME280 bme; 
MICS6814 gasSensor;
CCS811 co2Sensor(CCS811_ADDRESS);
INA3211 ina3221;
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();


PM25_AQI_Data data;


char json_data_string[MAX_JSON_SIZE]; 

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
  // --------- SETUP PM25AQI ---------- //
  Serial1.begin(9600, SERIAL_8N1, 17, 18);
  //pmSerial.begin(9600);
  // There are 3 options for connectivity!
  //if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
  if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
  //if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    log_d("Could not find PM 2.5 sensor!");
    int errCount = 0;
    while (errCount<10){
      delay(1000);
      errCount++; //
    } //Hang if there was a problem.
  }

  log_d("PM25 found!");






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

  aqi.read(&data);

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
  Serial.println("Volatile Organic Compounds = " + String(co2Sensor.getTVOC()) + " [ppb]");
  
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


  
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (environmental)"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));



}




void to_json(char * json){

  DynamicJsonDocument doc(MAX_JSON_SIZE);
  
  JsonObject epoc = doc["epoc"].to<JsonObject>();
  epoc["Giorno"] = String(rtc.getDay());
  epoc["Mese"] = String(rtc.getMonth()+1);
  epoc["Anno"] = String(rtc.getYear());
  epoc["Ore"] = String(rtc.getTime());
  epoc["count"] = String(count);
  
  JsonObject bme = doc["bme"].to<JsonObject>();
  bme["temperature"] = String(bme280.readTemperature(), 2);
  bme["humidity"] = String(bme280.readHumidity(), 0);
  bme["Pressure"] = String(bme280.readPressure()/ 100.0F, 2);
  
  JsonObject wind = doc["wind"].to<JsonObject>();
  wind["direction"] = "N";
  wind["intensity"] = String(10.0, 2);
  doc["rain"]["quantity"] = String(10.0, 2);
  
  doc["soil"]["moisture"] = String(10.0, 2);
  
  JsonObject air_quality = doc["air_quality"].to<JsonObject>();
  air_quality["co"] = String(mics.CO, 2);
  air_quality["co2"] = String(ccs811.getCO2(), 2);
  air_quality["nox"] = String(ccs811.getTVOC(), 2);
  air_quality["nh3"] = String(mics.NH3, 2);
  air_quality["no2"] = String(mics.NO2, 2);
  air_quality["pm10"] = String(pm.pm100_standard);
  air_quality["pm2.5"] = String(pm.pm25_standard);
  
  JsonObject power_managment = doc["power_managment"].to<JsonObject>();
  
  power_managment["power_pv"] = String(ina.power[0], 2);
  power_managment["power_bat"] = String((ina.power[1]*TIME_ALIVE/TIME_TO_SLEEP+16), 2);
  power_managment["power_app"] = String(ina.power[2]*TIME_ALIVE/TIME_TO_SLEEP+16, 2);
  power_managment["SOC"] = String(ina.soc);
  power_managment["bat_volt"] = String(ina.voltage[1]/1000, 2);
  
  // doc.shrinkToFit();  // optional
  
  serializeJson(doc, json, MAX_JSON_SIZE);
  
  serializeJson(doc, Serial);
  
  }