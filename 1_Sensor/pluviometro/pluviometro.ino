#define AREA_PLUVIOMETRO 63.25 //11.5 x 5.5 = 63.25 cm2
#define AREA_VASCHETTA 3 //3 ml ogni iterazione
#define CONVERSIONE_PLUVIOMETRO 0.4743 // 3 x 10^-3 /( 63.25 x 10^-6) = 3 x 10^3 / 63.25 = 47.43  [mm h oppure Litri per m2 ora]

#define CYCLE_RAIN_LENGTH 10 // tempo in secondi

int rainSensorPin = 40; // Pin a cui è collegato il sensore a effetto Hall
volatile int rainDroppCount = 0;


void setup() {
  Serial.begin(115200);
  
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);  // turn the LED on (HIGH is the voltage level)

  attachInterrupt(digitalPinToInterrupt(rainSensorPin), countTray, FALLING);
}




void loop() {

  float NIterazioniOra = rainDroppCount *  3600 / CYCLE_RAIN_LENGTH;

  float LitriMetroOra = NIterazioniOra * CONVERSIONE_PLUVIOMETRO;
  Serial.print("LitriMetroOra: " + String(LitriMetroOra) + "mm \n");

  rainDroppCount = 0;
  delay(CYCLE_RAIN_LENGTH*1000); // Attende mezzo secondo prima di ripetere il ciclo
}


void countTray() {
  rainDroppCount++;
}



/*
void loop()
{
  float NIterazioniOra = 10;
  float LitriMetroOra = NIterazioniOra * CONVERSIONE_PLUVIOMETRO;
  float mMora = LitriMetroOra;
  delay(1000);
}
*/