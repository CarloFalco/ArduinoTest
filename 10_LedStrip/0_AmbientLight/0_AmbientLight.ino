#include <Adafruit_NeoPixel.h>

#define NUM_LEDS_1 52
#define DATA_PIN_1 6
#define NUM_LEDS_2 8
#define DATA_PIN_2 5
#define BUTTON_PIN 3 // Pin digitale collegato al pulsante

// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 115200

// Delay for debouncing in milliseconds
#define DEBOUNCE_DELAY 50 


// Initialise LED-array
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_2, DATA_PIN_2, NEO_GRB + NEO_KHZ800);


 // The last time the output pin was toggled
unsigned long lastDebounceTime = 0;
 // The previous reading from the input pin
int lastButtonState = HIGH;
 // The current reading from the input pin
int buttonState = HIGH;
 // Livelli di luminosità: spento, 25%, 75%
int brightnessLevels[] = {0, 64, 192};
// Livello di luminosità corrente
int currentLevel = 0; 

unsigned long previousMillis = 0; // Memorizza l'ultimo tempo in cui il LED è stato aggiornato
const long interval = 1000; // Intervallo di lampeggio (in millisecondi)



uint8_t prefix[] = {'A', 'd', 'a'};
uint8_t hi, lo, chk, i;
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_1, DATA_PIN_1, NEO_GRB + NEO_KHZ800);



enum State {
  WAITING_FOR_MAGIC_WORD,
  READING_HEADER,
  READING_DATA
};



State currentState = WAITING_FOR_MAGIC_WORD;
uint8_t dataBuffer[NUM_LEDS_1 * 3];
uint8_t dataIndex = 0;

void setup() {
  strip1.begin();
  strip1.show();
  
  strip1.fill(strip1.Color(255, 0, 0), 0, NUM_LEDS_1);
  strip1.show();
  delay(500);
  strip1.fill(strip1.Color(0, 255, 0), 0, NUM_LEDS_1);
  strip1.show();
  delay(500);
  strip1.fill(strip1.Color(0, 0, 255), 0, NUM_LEDS_1);
  strip1.show();
  delay(500);
  strip1.fill(strip1.Color(0, 0, 0), 0, NUM_LEDS_1);
  strip1.show();
  
  Serial.begin(serialRate);


  pinMode(BUTTON_PIN, INPUT); // Configura il pin del pulsante come input con pull-up interno
  strip2.begin();
  //setStripBrightness(brightnessLevels[currentLevel]);
  strip2.show(); // Inizializza tutti i pixel a 'off'


  Serial.print("Ada\n");
}

void loop() {
  if (Serial.available()) {
    switch (currentState) {
      case WAITING_FOR_MAGIC_WORD:
        if (Serial.read() == prefix[i]) {
          i++;
          if (i == sizeof(prefix)) {
            currentState = READING_HEADER;
            i = 0;
          }
        } else {
          i = 0;
        }
        break;

      case READING_HEADER:
        if (Serial.available() >= 3) {
          hi = Serial.read();
          lo = Serial.read();
          chk = Serial.read();
          if (chk == (hi ^ lo ^ 0x55)) {
            currentState = READING_DATA;
            dataIndex = 0;
          } else {
            currentState = WAITING_FOR_MAGIC_WORD;
          }
        }
        break;

      case READING_DATA:
        if (Serial.available() >= 3) {
          dataBuffer[dataIndex++] = Serial.read();
          dataBuffer[dataIndex++] = Serial.read();
          dataBuffer[dataIndex++] = Serial.read();
          if (dataIndex >= sizeof(dataBuffer)) {
            for (uint8_t j = 0; j < NUM_LEDS_1; j++) {
              strip1.setPixelColor(j, strip1.Color(dataBuffer[j * 3], dataBuffer[j * 3 + 1], dataBuffer[j * 3 + 2]));
            }
            strip1.show();
            currentState = WAITING_FOR_MAGIC_WORD;
          }
        }
        break;
    }
  }

  // Read the state of the switch into a local variable:
  int reading = digitalRead(BUTTON_PIN);
  // Check if the button state has changed
  if (reading != lastButtonState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // Whatever the reading is at, it's been there for longer than the debounce delay
    // so take it as the actual current state:
    if (reading != buttonState) {
      buttonState = reading;
      // Only toggle the LED if the new button state is LOW
      if (buttonState == HIGH) {
        currentLevel++;
        currentLevel = (currentLevel > 2) ? 0 : currentLevel;
      setStripBrightness(&strip2, brightnessLevels[currentLevel]);
      }
    }
  }
  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;


}


void setStripBrightness(Adafruit_NeoPixel* strip, int brightness) {
  Serial.println(brightness);
  for(int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, strip->Color(brightness, brightness, brightness)); // Imposta il colore bianco con la luminosità desiderata
  }
  strip->show(); // Aggiorna la striscia LED
}