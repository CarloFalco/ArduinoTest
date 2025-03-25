#ifndef LED_H
#define LED_H

#include <Arduino.h>


class LEDControl {
  public:

    LEDControl(int pin = 97) : _pin(pin){
    }
    void begin(){
      pinMode(_pin, OUTPUT);
    }
    void accendi() {
      digitalWrite(_pin, HIGH);
    }

    void spegni() {
      digitalWrite(_pin, LOW);
    }

  protected:
    int _pin;
};




class LampeggianteLEDControl : public LEDControl {
  public:
    LampeggianteLEDControl(int pin, int intensity) : LEDControl(pin), _intensity(intensity) {
      // do stuf here
    }
    // Ridefinizione del metodo accendi()
    void accendi() {
      // Implementazione personalizzata per LampeggianteLEDControl
      Serial.println("LampeggianteLEDControl: Accensione speciale!");
      neopixelWrite(_pin,_intensity,_intensity,_intensity);
    }
    void blu() {
      neopixelWrite(_pin,0,0,_intensity);
    }    
    void red() {
      neopixelWrite(_pin,_intensity,0,0);
    }
    void green() {
      neopixelWrite(_pin,0,_intensity,0);
    }

  private:
    int _intensity;
};



class MYLED {
  public:

    MYLED(void):_pin(0){

    }

    void begin(int pin){
      _pin = pin;
      pinMode(_pin, OUTPUT);
    }

    void accendi() {
      digitalWrite(_pin, HIGH);
    }

    void spegni() {
      digitalWrite(_pin, LOW);
    }

  protected:
    int _pin;
};



#endif



//  neopixelWrite(LED_BUILTIN,0,0,0);
  
// neopixelWrite(LED_BUILTIN,0,64,0);