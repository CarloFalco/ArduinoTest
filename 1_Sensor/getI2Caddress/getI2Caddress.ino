#include <Wire.h>
#define PIN_5V 4
#define PIN_3V 11




void setup() {
  pinMode(PIN_3V, OUTPUT);
  digitalWrite(PIN_3V, HIGH);  // turn the 3.3V
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  // turn the 5V
  pinMode(41, OUTPUT);
  digitalWrite(41, LOW);  // turn WAK Pin low

  delay(300);
  
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Attendi che la porta seriale sia pronta
  Serial.println("\nI2C Scanner");
}

void loop() {


  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // Aspetta 5 secondi prima di ripetere la scansione
}