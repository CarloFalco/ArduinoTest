#ifndef _TYPE_CONVERSION_H_
#define _TYPE_CONVERSION_H_


#define S_TO_uS_FACTOR 1000000UL                /* Conversion factor for seconds to micro seconds */
#define uS_TO_S_FACTOR (1.0 / S_TO_uS_FACTOR)   /* Conversion factor for micro seconds to seconds */

#define S_TO_mS_FACTOR 1000ULL                  /* Conversion factor for seconds to milli seconds */
#define mS_TO_S_FACTOR (1.0 / S_TO_mS_FACTOR)   /* Conversion factor for milli seconds to seconds */

#define mS_TO_uS_FACTOR 1000ULL  /* Conversion factor for milli seconds to micro seconds */


#define h_TO_S_FACTOR 3600   /* Conversion factor for micro seconds to seconds */


#define m_TO_mm_FACTOR 1000ULL    /* Conversion factor for meter to millimeter */
#define mm_TO_m_FACTOR (1.0/m_TO_mm_FACTOR)    /* Conversion factor for millimeter to meter */


/* https://learn.microsoft.com/it-it/cpp/c-runtime-library/math-constants?view=msvc-170 */
#define M_PI    3.141592        /* PIGRECO*/
#define M_1_PI 	0.31831         /* 1/PIGRECO*/
#define M_SQRT2 1.4142          /* sqrt(2)*/



// POWERLINE

#define PIN_5V GPIO_NUM_4
#define PIN_3V GPIO_NUM_11


/* qui potrei metterci la configurazione dei sensori? */

// #define DEBUG_SENSOR // enable serial print for debug

// ANEMOMETER
#define PIN_ANEMOMETER GPIO_NUM_19

#define ANEMOMETER_DIAMETER 80 * 2 * mm_TO_m_FACTOR
#define TASK_ANEMOMETER 200  // Periodo del task in millisecondi


// WINDVANE
#define AS5600_ADDRESS 0x36
#define RAW_ANGLE 0x0C



// RAINGAUGE
#define PIN_RAINGAUGE GPIO_NUM_20


#define AREA_PLUVIOMETRO 63.25 //11.5 x 5.5 = 63.25 cm2
#define AREA_VASCHETTA 3 //3 ml ogni iterazione
#define CONVERSIONE_PLUVIOMETRO ((AREA_VASCHETTA)/(AREA_PLUVIOMETRO)*1000) // (3) 3 x 10^-3 /( 63.25 x 10^-6) = 3 x 10^3 / 63.25 = 47.43  [mm h oppure Litri per m2 ora]

#define CYCLE_RAIN_LENGTH 10 // tempo in secondi


// TERMOMETER
#define BME280_ADDRESS 0x76


// GASSENSOR MICS6814   
#define PIN_CO  ADC1_CHANNEL_4
#define PIN_NO2 ADC1_CHANNEL_5
#define PIN_NH3 ADC1_CHANNEL_6


// GASSENSOR CCS811
#define CCS811_ADDRESS 0x5A
#define PIN_WAK GPIO_NUM_41


// il pin wak va messo a maasa per accenderlo

// GASSENSOR INA
#define INA_ADDRESS 0x40


// SCHEDULER INFORMATION
#define SLEEP_TIME 10 // TIME since next wake up [s]
#define AWAKE_TIME 60






#define MAX_NH3 35
#define MIN_NH3 25

#define MAX_NO2 0.021
#define MAX_NO2 0.1

#define MAX_CO2 15000
#define MIN_CO2 5000

#define MAX_CO 100
#define MIN_CO 20


// 0,3 mg/m³ ; 0,3 - 0,5 mg/m³; 0,5 - 1 mg/m³, 1 - 3 mg/m³, Oltre 3 mg/m³
#define MAX_TVOC 600
#define MIN_TVOC 100


#endif
