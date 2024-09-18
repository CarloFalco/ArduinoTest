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
  neopixelWrite(pin,64,64,64);
  sts = 1;
  count.increase();
}
void Led::off() {
  neopixelWrite(pin,0,0,0);
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
  neopixelWrite(pin,luminosity,0,0);
}
void Led::green(){
  neopixelWrite(pin,0,luminosity,0);
}
void Led::blue(){
  neopixelWrite(pin,0,0,luminosity);
}

int Led::prevVal(){
  return digitalRead(pin);
}


int Led::pinStatus(){
  return sts;
}