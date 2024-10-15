#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Utilities.h"

void setup_rtc_time(ESP32Time *);
void check_DST(ESP32Time *);

void do_eprom_read();
void do_eprom_write();


typedef struct {
  String Eeprom_sData;  
  String Eeprom_sArray[4];          
  bool Eeprom_bArray[4];    

  bool Eeprom_bData1;
  bool Eeprom_bData2;
  bool Eeprom_bData3;    
  int Eeprom_iData1;        // quantità di cibo
  int eprom_good;        // quantità di cibo
} Eeprom_Data_Type;

extern Eeprom_Data_Type eepromData;



#endif
