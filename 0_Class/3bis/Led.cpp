#include "Led.h"





// Costruttore
LEDControl::LEDControl(int pin) : _pin(pin) {}

// Metodo di inizializzazione
void LEDControl::begin() {
    pinMode(_pin, OUTPUT);
}

// Accendi il LED
void LEDControl::accendi() {
    digitalWrite(_pin, HIGH);
}

// Spegni il LED
void LEDControl::spegni() {
    digitalWrite(_pin, LOW);
}



// Costruttore
LampeggianteLEDControl::LampeggianteLEDControl(int pin, int intensity) 
    : LEDControl(pin), _intensity(intensity) {

    }

// Ridefinizione del metodo accendi()
void LampeggianteLEDControl::accendi() {
    Serial.println("LampeggianteLEDControl: Accensione speciale!");
    neopixelWrite(_pin, _intensity, _intensity, _intensity);
}

// Metodo per impostare il colore blu
void LampeggianteLEDControl::blu() {
    neopixelWrite(_pin, 0, 0, _intensity);
}

// Metodo per impostare il colore rosso
void LampeggianteLEDControl::red() {
    neopixelWrite(_pin, _intensity, 0, 0);
}

// Metodo per impostare il colore verde
void LampeggianteLEDControl::green() {
    neopixelWrite(_pin, 0, _intensity, 0);
}


/*

MYLED::MYLED(void)
:_pin(0){
}

void MYLED::begin(int pin){
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void MYLED::accendi() {
  digitalWrite(_pin, HIGH);
}

void MYLED::spegni() {
  digitalWrite(_pin, LOW);
}
*/