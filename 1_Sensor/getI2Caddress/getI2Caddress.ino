#include <Wire.h>

void setup() {
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(300);
  
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Attendi che la porta seriale sia pronta
  Serial.println("\nI2C Scanner");
}

void loop() {
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)

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