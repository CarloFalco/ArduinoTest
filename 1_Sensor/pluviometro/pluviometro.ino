

#define AREA_PLUVIOMETRO 63.25 //11.5 x 5.5 = 63.25 mm2
#define AREA_VASCHETTA 3 //3 ml ogni iterazione
#define CONVERSIONE_PLUVIOMETRO 0.4743 // 3 x 10^3 /( 63.25 x 10^-4) = 3 x 10 / 63.25 = 0.4743 


void setup()
{

}


void loop()
{
  float NIterazioniOra = 10;
  float LitriMetroOra = NIterazioniOra * CONVERSIONE_PLUVIOMETRO;
  float mMora = LitriMetroOra;
  delay(1000);
}
