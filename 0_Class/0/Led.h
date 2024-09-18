#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "count.h"

extern Count count;

class Led {
  
  private:
    byte pin;
    int sts;
    int prevVal();
    
  public:
    // Led() {} // do not use
    Led(byte pin);

    void init();
    void init(byte defaultState);

    void on();
    void off();

    void red();
    void green();
    void blue();

    void toggle();
    
    int pinStatus();
};

#endif