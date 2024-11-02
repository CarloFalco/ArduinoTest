/*
 * Arduino interface for the use of WS2812 strip LEDs
 * Uses Adalight protocol and is compatible with Boblight, Prismatik etc...
 * "Magic Word" for synchronisation is 'Ada' followed by LED High, Low and Checksum
 * @author: Wifsimster <wifsimster@gmail.com> 
 * @library: Adafruit_NeoPixel
 * @date: 11/22/2015
 */
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 52
#define DATA_PIN 6

// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 115200

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

// Initialise LED-array
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // Initial RGB flash
  strip.fill(strip.Color(255, 0, 0), 0, NUM_LEDS);
  strip.show();
  delay(500);
  strip.fill(strip.Color(0, 255, 0), 0, NUM_LEDS);
  strip.show();
  delay(500);
  strip.fill(strip.Color(0, 0, 255), 0, NUM_LEDS);
  strip.show();
  delay(500);
  strip.fill(strip.Color(0, 0, 0), 0, NUM_LEDS);
  strip.show();
  
  Serial.begin(serialRate);
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
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
  
  // Clear the strip
  strip.clear();
  
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  
  // Shows new values
  strip.show();
}


