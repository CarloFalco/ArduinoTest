#define LED_BUILTIN 97
#define RGB_BRIGHTNESS 100

#include "Led.h"


Led led(LED_BUILTIN);


void setup(){
  Serial.begin(115200);
  led.init();
}


void loop(){

    delay(500);
  led.toggle();

  delay(500);
  led.red(); 

  delay(500);
  led.green();  

  delay(500);
  led.blue();  


}


