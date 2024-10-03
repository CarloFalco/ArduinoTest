#include <EEPROM.h>

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

Eeprom_Data_Type eepromData;



void setup() {
  // Inizializzo la mia struttura
  Serial.begin(115200);

  do_eprom_read();
  Serial.print("EEPROM 1: ");
  Serial.println(eepromData.eprom_good);
  delay(1000);
  do_eprom_read();
}

void loop() {

  
}



// FUNZIONI RELATIVE ALLA EEPROM
void do_eprom_read() {

  EEPROM.begin(200);
  EEPROM.get(0, eepromData);

  Serial.print("EEPROM STS_Before: ");
  Serial.println(eepromData.eprom_good);

  if (eepromData.eprom_good == 14) {
    Serial.println("Good settings in the EPROM ");
    Serial.print("EEPROM STS: ");
    Serial.println(eepromData.eprom_good);
    // eepromData.Eeprom_bArray[i] // go access to information inside the struct

  } else {
    Serial.println("EPROM canot be read ");
    eepromData = {
      String(""),
      { String("CHAT_ID_1"), String("CHAT_ID_2"), String(""), String("") },
      { 1, 0, 0, 0 },
      false,                // Boolean can bee true or false, or LOW or HIGH
      false,
      LOW, 
      10,      
      14   
    };
    do_eprom_write();
  }
}

void do_eprom_write() {
  Serial.println("Writing to EPROM ...");
  EEPROM.begin(200);
  EEPROM.put(0, eepromData);
  EEPROM.commit();
  EEPROM.end();
}