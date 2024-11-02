/*
 * Arduino interface for the use of WS2812 strip LEDs
 * Uses Adalight protocol and is compatible with Boblight, Prismatik etc...
 * "Magic Word" for synchronisation is 'Ada' followed by LED High, Low and Checksum
 * @author: Wifsimster <wifsimster@gmail.com> 
 * @library: Adafruit_NeoPixel
 * @date: 11/22/2015
 */
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

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;


// Initialise LED-array
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_1, DATA_PIN_1, NEO_GRB + NEO_KHZ800);

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


void setup() {
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
  
  // Initial RGB flash for strip1
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
  
  // Set strip2 to white
  strip2.fill(strip2.Color(255, 255, 255), 0, NUM_LEDS_2);
  strip2.show();
  
  Serial.begin(serialRate);
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
  // Set the button pin as input with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT); 
}

void loop() { 
  // Wait for first byte of Magic Word
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  
  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  strip1.clear();
  
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS_1; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    strip1.setPixelColor(i, strip1.Color(r, g, b));
  }
  
  strip1.show();




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
        if (currentLevel > 4){currentLevel = 1;}

      setStripBrightness(&strip2, brightnessLevels[currentLevel]);
      }
    }
  }

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;





}


void setStripBrightness(Adafruit_NeoPixel* strip, int brightness) {
  for(int i = 0; i < strip->numPixels(); i++) {
    strip->setPixelColor(i, strip->Color(brightness, brightness, brightness)); // Imposta il colore bianco con la luminosità desiderata
  }
  strip->show(); // Aggiorna la striscia LED
}
