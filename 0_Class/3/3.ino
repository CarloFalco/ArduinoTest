#define LED_BUILTIN 97
#include "Led.h"

/*
LEDControl led1(13);
LampeggianteLEDControl led2(12, 500);
*/




LampeggianteLEDControl ledNew(LED_BUILTIN, 64);
// MYLED myled;


void setup() {
  // Nessuna configurazione necessaria
  Serial.begin(115200);
  ledNew.begin();
}

void loop() {
  
  ledNew.accendi();
  delay(1000);  
  ledNew.spegni();
  delay(1000);
}


