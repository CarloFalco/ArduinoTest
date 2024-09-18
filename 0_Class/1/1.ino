#define LED_BUILTIN 97
#define RGB_BRIGHTNESS 100

#include "Led.h"
#include "count.h"


Led led(LED_BUILTIN);
Count count(10);


void setup(){
  Serial.begin(115200);
  led.init();
}


void loop(){

  
  delay(500);
  led.toggle();
  Serial.print("valore del counter: " + String(count.numVal()) + "\n");

  delay(500);
  led.red(); 

  delay(500);
  led.green();  

  delay(500);
  led.blue();  

}

