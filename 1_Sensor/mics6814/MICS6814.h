#ifndef MICS6814_H
#define MICS6814_H

#include "Arduino.h"
#include <driver/adc.h>

typedef enum{
  CH_NH3, CH_RED, CH_OX
} channel_t;

typedef enum{
  CO, NO2, NH3, C3H8, C4H10, CH4, H2, C2H5OH
} gas_t;

typedef struct {
  float CO = 0;
  float NO2 = 0;
  float NH3 = 0;
  float C3H8 = 0;
  float C4H10 = 0;
  float CH4 = 0;
  float H2 = 0;
  float C2H5OH = 0;
} MICS;



class MICS6814{

public:
	MICS6814();

	int begin();
	float measureMICS(gas_t gas);
  void getAll(MICS& , int);


private:
	uint16_t getResistance    (channel_t channel) const;
	uint16_t getBaseResistance(channel_t channel) const;
	float    getCurrentRatio  (channel_t channel) const;


	uint16_t NH3baseR;
	uint16_t REDbaseR;
	uint16_t OXbaseR;
};

#endif