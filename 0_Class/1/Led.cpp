#include "Led.h"


Led::Led(byte pin){
  this->pin = pin;
}

void Led::init(){
  pinMode(pin, OUTPUT);
  off();
}

void Led::init(byte defaultState){
  pinMode(pin, OUTPUT);
  if (defaultState==HIGH){
    on();
  }else{
    off();
  }
}

void Led::on() {
  digitalWrite(pin, HIGH);
  sts = 1;
  count.increase();
}
void Led::off() {
  digitalWrite(pin, LOW);
  sts = 0;
  count.decrease();
}

void Led::toggle(){
  /*
  if (sts == 1){
    off();
  }else{
    on();
  }
  */
  int sts_led = prevVal();
  if (sts_led == 1){
    off();
  }else{
    on();
  }  
}

void Led::red(){
  neopixelWrite(RGB_BUILTIN,64,0,0);
}
void Led::green(){
  neopixelWrite(RGB_BUILTIN,0,64,0);
}
void Led::blue(){
  neopixelWrite(RGB_BUILTIN,0,0,64);
}

int Led::prevVal(){
  return digitalRead(pin);
}


int Led::pinStatus(){
  return sts;
}