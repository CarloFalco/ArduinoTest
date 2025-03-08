// CO

// Definizione dei pin dei sensori
const int pinNO2 = A3;
const int pinNH3 = A4;
const int pinCO = A5;

// Variabili per memorizzare i valori letti dai sensori
float valueNH3 = 0;
float valueNO2 = 0;
float valueCO = 0;


#define LED_BUILTIN 97

// the setup function runs once when you press reset or power the board
void setup() {
  
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);  // turn the 3.3V
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the 3.3V


  Serial.begin(115200);
  analogReadResolution(12);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);


}

// the loop function runs over and over again forever
void loop() {

/*
  int rawValue = analogRead(pinNO2);
  float Value = rawValue * (3.3 / 4095.0);
  int val = map(rawValue, 0, 4095.0, 0, 3300);
*/


  valueNH3 = map(analogRead(pinNH3), 0, 4095.0, 1, 500);
  valueNO2 = (map(analogRead(pinNO2), 0, 4095.0, 1, 1000))/100.0;
  valueCO = map(analogRead(pinCO), 0, 4095.0, 1, 1000);

  Serial.print("NH3:");
  Serial.print(valueNH3);
  Serial.print(", ");
  Serial.print("NO2:");
  Serial.print(valueNO2);
  Serial.print(", ");
  Serial.print("CO:");
  Serial.println(valueCO);




  //digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //delay(1000);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
  Serial.println("");
}


