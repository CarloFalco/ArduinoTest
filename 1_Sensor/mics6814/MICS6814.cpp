#include "MICS6814.h"
#include <driver/adc.h>

#define maxIter 2
#define maxIterCalib 400

// RED -> CO -> ADC1_CHANNEL_4
// NH3 -> NH3 -> ADC1_CHANNEL_5
// OX -> NO2 -> ADC1_CHANNEL_6

#define PIN_RED ADC1_CHANNEL_4
#define PIN_NH3 ADC1_CHANNEL_5
#define PIN_OX ADC1_CHANNEL_6

MICS6814 :: MICS6814 (){

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PIN_NH3, ADC_ATTEN_DB_11); 
  adc1_config_channel_atten(PIN_RED, ADC_ATTEN_DB_11); 
  adc1_config_channel_atten(PIN_OX, ADC_ATTEN_DB_11); 


}

int MICS6814 :: begin (){
  // Continuously measure the resistance,
  // storing the last N measurements in a circular buffer.
  // Calculate the floating average of the last seconds.
  // If the current measurement is close to the average stop.

  // Seconds to keep stable for successful calibration
  // (Keeps smaller than 64 to prevent overflows)
  uint8_t seconds = 10;
  // Allowed delta for the average from the current value
  uint8_t delta = 2;
  int count = 0;
  // Circular buffer for the measurements
  uint16_t bufferNH3[seconds];
  uint16_t bufferRED[seconds];
  uint16_t bufferOX[seconds];
  // Pointers for the next element in the buffer
  uint8_t pntrNH3 = 0;
  uint8_t pntrRED = 0;
  uint8_t pntrOX = 0;
  // Current floating sum in the buffer
  uint16_t fltSumNH3 = 0;
  uint16_t fltSumRED = 0;
  uint16_t fltSumOX = 0;

  // Current measurements;
  uint16_t curNH3;
  uint16_t curRED;
  uint16_t curOX;

  // Flag to see if the channels are stable
  bool NH3stable = false;
  bool REDstable = false;
  bool OXstable = false;

  // Initialize buffer
  for (int i = 0; i < seconds; ++i) {
    bufferNH3[i] = 0;
    bufferRED[i] = 0;
    bufferOX[i] = 0;
  }

  do {
    // Wait a second
    delay(500);
    Serial.print(".");
    // Read new resistances
    unsigned long rs = 0;
    for (int i = 0; i < 3; i++) {
    delay(2);
    rs += (adc1_get_raw(PIN_NH3)+1)/4-1;
    }
    curNH3 = rs/3;
    rs = 0;
    for (int i = 0; i < 3; i++) {
    delay(2);
    rs += (adc1_get_raw(PIN_RED)+1)/4-1;
    }
    curRED = rs/3;
    rs = 0;
    for (int i = 0; i < 3; i++) {
    delay(2);
    rs += (adc1_get_raw(PIN_OX)+1)/4-1;
    }
    curOX = rs/3;

    // Update floating sum by subtracting value
    // about to be overwritten and adding the new value.
    fltSumNH3 = fltSumNH3 + curNH3 - bufferNH3[pntrNH3];
    fltSumRED = fltSumRED + curRED - bufferRED[pntrRED];
    fltSumOX = fltSumOX + curOX - bufferOX[pntrOX];

    // Store new measurement in buffer
    bufferNH3[pntrNH3] = curNH3;
    bufferRED[pntrRED] = curRED;
    bufferOX[pntrOX] = curOX;

    // Determine new state of flags
    NH3stable = abs(fltSumNH3 / seconds - curNH3) < delta;
    REDstable = abs(fltSumRED / seconds - curRED) < delta;
    OXstable = abs(fltSumOX / seconds - curOX) < delta;

    // Advance buffer pointer
    pntrNH3 = (pntrNH3 + 1) % seconds ;
    pntrRED = (pntrRED + 1) % seconds;
    pntrOX = (pntrOX + 1) % seconds;

  count += 1;
  } while (( !NH3stable || !REDstable || !OXstable ) && count < maxIterCalib);

  NH3baseR = fltSumNH3 / seconds;
  REDbaseR = fltSumRED / seconds;
  OXbaseR = fltSumOX / seconds;

  Serial.print("NH3:");
  Serial.print(NH3baseR);
  Serial.print(", ");
  Serial.print("NO2:");
  Serial.print(OXbaseR);
  Serial.print(", ");
  Serial.print("CO:");
  Serial.println(REDbaseR);

  if (count > maxIterCalib-1){
    Serial.print("Calibration Fail");
    return 0;
  }else{
    return 1;
  }
}

float MICS6814 :: measureMICS (gas_t gas){
	float ratio;
	float c = 0;
  unsigned long rawPin = 0;
  float rawVolt = 0;

	switch (gas){
    case CO: // Carbon Monoxide, a chemical compound composed of one carbon atom and one oxygen atom.
      ratio = getCurrentRatio (CH_RED);
      c = pow (ratio, -1.179) * 4.385;


      
      for (int i = 0; i < 3; i++) {
      delay(10);
      rawPin += (adc1_get_raw(PIN_RED));
      }
      rawPin = rawPin/3;
      rawVolt = map(rawPin, 0, 4096, 0, 3300);
      Serial.print("\n");
      Serial.print("Volt:");
      Serial.println(rawPin);

      break;
    case NO2: // Nitrogen Dioxide, a chemical compound composed of one nitrogen atom and two oxygen atoms.
      ratio = getCurrentRatio(CH_OX);
      c = pow(ratio, 1.007) / 6.855;

      for (int i = 0; i < 3; i++) {
        delay(10);
        rawPin += (adc1_get_raw(PIN_OX));
      }
      rawPin = rawPin/3;
      rawVolt = map(rawPin, 0, 4096, 0, 3300);
      Serial.print("\n");
      Serial.print("Volt NO2:");
      Serial.println(rawVolt);


      break;
    case NH3: // Ammonia, a chemical compound composed of one nitrogen atom and three hydrogen atoms.
      ratio = getCurrentRatio (CH_NH3);
      c = pow (ratio, -1.67) / 1.47;

      for (int i = 0; i < 3; i++) {
        delay(10);
        rawPin += (adc1_get_raw(PIN_NH3));
      }
      rawPin = rawPin/3;
      rawVolt = map(rawPin, 0, 4096, 0, 3300);
      Serial.print("\n");
      Serial.print("Volt NH3:");
      Serial.println(rawVolt);


      break;
    case C3H8: // Propane, a hydrocarbon compound consisting of three carbon atoms and eight hydrogen atoms.
      ratio = getCurrentRatio(CH_NH3);
      c = pow(ratio, -2.518) * 570.164;
      break;
    case C4H10: // Butane, another hydrocarbon compound consisting of four carbon atoms and ten hydrogen atoms.
      ratio = getCurrentRatio(CH_NH3);
      c = pow(ratio, -2.138) * 398.107;
      break; 
    case CH4: // Methane, a simple hydrocarbon compound composed of one carbon atom and four hydrogen atoms.
      ratio = getCurrentRatio(CH_RED);
      c = pow(ratio, -4.363) * 630.957;
      break;
    case H2: // Hydrogen, a hydrogen molecule composed of two hydrogen atoms covalently bonded.
      ratio = getCurrentRatio(CH_RED);
      c = pow(ratio, -1.8) * 0.73;
      break;
    case C2H5OH: // Ethanol, a chemical compound also known as ethyl alcohol, composed of two carbon atoms, six hydrogen atoms, and one oxygen atom.
      ratio = getCurrentRatio(CH_RED);
      c = pow(ratio, -1.552) * 1.622;
      break;
    }

	return isnan (c)? -1: c;
}

uint16_t MICS6814 :: getResistance (channel_t channel) const{
	unsigned long rs = 0;
	int counter = 0;

	switch (channel)
	{
	case CH_RED:
		for (int i = 0; i <maxIter; i ++)
		{
			rs += (adc1_get_raw(PIN_RED)+1)/4-1;
			counter ++;
			delay (2);
		}
	case CH_OX:
		for (int i = 0; i <maxIter; i ++)
		{
			rs += (adc1_get_raw(PIN_OX)+1)/4-1;
			counter ++;
			delay (2);
		}
	case CH_NH3:
		for (int i = 0; i <maxIter; i ++)
		{
			rs += (adc1_get_raw(PIN_NH3)+1)/4-1;
			counter ++;
			delay (2);
		}
	}

	return counter != 0 ? rs / counter: 0;
}

uint16_t MICS6814 :: getBaseResistance (channel_t channel) const{

  switch (channel) {
    case CH_NH3:
      return NH3baseR;
    case CH_RED:
      return REDbaseR;
    case CH_OX:
      return OXbaseR;
  }
  //  }
  
  return 0;
}

float MICS6814 :: getCurrentRatio (channel_t channel) const{
	float baseResistance = (float) getBaseResistance (channel);
	float resistance = (float) getResistance (channel);

	return resistance / baseResistance * (4095.0 - baseResistance) / (4095.0 - resistance);

	return -1.0;
}

float MICS6814::getCO(){
  return measureMICS(CO);
}

float MICS6814::getNO2(){
  return measureMICS(NO2);
}

float MICS6814::getNH3(){    
  return measureMICS(NH3);
  
}

void MICS6814::getAll(MICS&  mics, int fullParam = 0){

  if (fullParam) {
    mics.CO = measureMICS(CO);     // Carbon Monoxide
    mics.NO2 = measureMICS(NO2);   // Nitrogen Dioxide
    mics.NH3 = measureMICS(NH3);    // Ammonia
    mics.C3H8 = measureMICS(C3H8);  // Propane
    mics.C4H10 = measureMICS(C4H10);// Butane
    mics.CH4 = measureMICS(CH4);    // Methane
    mics.H2 = measureMICS(H2);     // Hydrogen
    mics.C2H5OH = measureMICS(C2H5OH);     // Ethanol
  }else{
    mics.CO = measureMICS(CO);     // Carbon Monoxide
    mics.NO2 = measureMICS(NO2);   // Nitrogen Dioxide
    mics.NH3 = measureMICS(NH3);    // Ammonia
  } 



}