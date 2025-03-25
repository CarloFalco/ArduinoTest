
#include <driver/adc.h>
#include "esp_adc_cal.h"

uint32_t voltage;
uint32_t voltage2;
static esp_adc_cal_characteristics_t adc1_value;


double convert = 0.764102564; // 1000*(3.126/4095);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("a");
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_value);
  // adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12); 
  Serial.println("v");
  delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:


  voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_3), &adc1_value);
  voltage2 = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_4), &adc1_value);
  voltage2 = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_3), &adc1_value);
  Serial.print("a:");
  Serial.print(voltage2);
  Serial.print(", b:");
  Serial.println(voltage);


  /*
  int rs = 0;
  for (int i = 0; i <3; i ++){
    delay (1);
    rs += adc1_get_raw (ADC1_CHANNEL_3);
  }
  rs = rs/3;
  Serial.print("adcValue: ");
  Serial.println(rs);
  Serial.print("ADCVolt: ");
  Serial.println(rs*convert);
  delay(200);
  */
  delay(100);
}
		


