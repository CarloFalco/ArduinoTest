#ifndef LED_H
#define LED_H

#include <Arduino.h>


class LEDControl {
  public:

    LEDControl(int pin = 97);
    void begin();
    virtual void accendi();
    virtual void spegni();

  protected:
    int _pin;
};

// questa eredita da quella sopra
class LampeggianteLEDControl : public LEDControl {
  public:
    LampeggianteLEDControl(int pin, int intensity);
    
    void accendi() override;  // Ridefinisce accendi()
    void blu();   // Imposta il LED su blu
    void red();   // Imposta il LED su rosso
    void green(); // Imposta il LED su verde

  private:
    int _intensity;
};

/*
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


*/
#endif



//  neopixelWrite(LED_BUILTIN,0,0,0);
  
// neopixelWrite(LED_BUILTIN,0,64,0);