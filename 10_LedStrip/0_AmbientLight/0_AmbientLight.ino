#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 52
#define DATA_PIN 6

#define serialRate 115200

uint8_t prefix[] = {'A', 'd', 'a'};
uint8_t hi, lo, chk, i;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

enum State {
  WAITING_FOR_MAGIC_WORD,
  READING_HEADER,
  READING_DATA
};

State currentState = WAITING_FOR_MAGIC_WORD;
uint8_t dataBuffer[NUM_LEDS * 3];
uint8_t dataIndex = 0;

void setup() {
  strip.begin();
  strip.show();
  
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
            for (uint8_t j = 0; j < NUM_LEDS; j++) {
              strip.setPixelColor(j, strip.Color(dataBuffer[j * 3], dataBuffer[j * 3 + 1], dataBuffer[j * 3 + 2]));
            }
            strip.show();
            currentState = WAITING_FOR_MAGIC_WORD;
          }
        }
        break;
    }
  }
}
