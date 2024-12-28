#include <Wire.h>

#define AS5600_ADDRESS 0x36

#define RAW_ANGLE 0x0C

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Attendi che la porta seriale sia pronta
  Serial.println("AS5600 Magnet Position");
}

void loop() {
  uint16_t rawAngle = readRawAngle();
  float angle = (rawAngle * 360.0) / 4096.0; // Converti il valore grezzo in gradi
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" degrees \t");

  String direction = getDirection(angle);
  Serial.print("Direction: ");
  Serial.println(direction);



  delay(500); // Leggi la posizione ogni 500ms
}




uint16_t readRawAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(RAW_ANGLE);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDRESS, 2);
  uint16_t rawAngle = Wire.read() << 8 | Wire.read();
  return rawAngle;
}

String getDirection(float angle) {
  if (angle >= 337.5 || angle < 22.5) return "N";
  else if (angle >= 22.5 && angle < 67.5) return "NE";
  else if (angle >= 67.5 && angle < 112.5) return "E";
  else if (angle >= 112.5 && angle < 157.5) return "SE";
  else if (angle >= 157.5 && angle < 202.5) return "S";
  else if (angle >= 202.5 && angle < 247.5) return "SW";
  else if (angle >= 247.5 && angle < 292.5) return "W";
  else return "NW";
}