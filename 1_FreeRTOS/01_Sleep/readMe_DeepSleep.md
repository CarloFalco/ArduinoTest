# Datatype:
*esp_sleep_wakeup_cause_t wakeup_reason*; il tipo di dato che contiene il motivo del risveglio  
*wakeup_reason = esp_sleep_get_wakeup_cause()*; la funzione che serve per capire perche è andato in sleep



# modalità di deep sleep:

1. ESP_SLEEP_WAKEUP_EXT0: risveglio tramite un singolo pin GPIO.
1. ESP_SLEEP_WAKEUP_EXT1: risveglio tramite un set di pin GPIO.
1. ESP_SLEEP_WAKEUP_TIMER: risveglio tramite un timer hardware dopo un determinato intervallo di tempo.




### ESP_SLEEP_WAKEUP_EXT0
*esp_sleep_enable_ext0_wakeup(gpio_num_t gpio_num, int level)*;  

**gpio_num**: This function accepts as first argument the pin you want to use, in this format GPIO_NUM_X, in which X represents the GPIO number of that pin.  
**level**: The second argument, level, can be either 1 or 0. This represents the state of the GPIO that will trigger wake up.  

*pinMode(GPIO_NUM_33, INPUT_PULLDOWN);*  
*esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);*  
*esp_deep_sleep_start();*  


### ESP_SLEEP_WAKEUP_EXT1
Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.
  
*esp_sleep_enable_ext1_wakeup(bitmask, mode)*;  
**bitmask**: A bitmask of the GPIO numbers that will cause the wake up; eg:..

WAKEUP_PIN_1 = 33 e WAKEUP_PIN_2 = 34:
1ULL << 33: 0x0000000200000000 (bit 34° is 1).
1ULL << 34: 0x0000000400000000 (bit 35° is 1).
0x0000000200000000 | 0x0000000400000000 = 0x0000000600000000
Quindi, 0x0000000600000000 è la bitmask risultante, con il 34° e il 35° bit impostati a 1.


**mode**: Mode: the logic to wake up the ESP32. It can be:
- ESP_EXT1_WAKEUP_ALL_LOW/HIGH: wake up when all GPIOs go low/high;
- ESP_EXT1_WAKEUP_ANY_LOW/HIGH: wake up if any of the GPIOs go low/high.

*pinMode(GPIO_NUM_33, INPUT_PULLDOWN);*  
*esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);* 
*esp_deep_sleep_start();



/*



ESEMPIO CREAZIONE BITMASK
Supponiamo che WAKEUP_PIN_1 = 33 e WAKEUP_PIN_2 = 34:
1ULL << 33: Risultato = 0x0000000200000000 (il 34° bit è 1).
1ULL << 34: Risultato = 0x0000000400000000 (il 35° bit è 1).
OR dei due valori:
0x0000000200000000 | 0x0000000400000000 = 0x0000000600000000
Quindi, 0x0000000600000000 è la bitmask risultante, con il 34° e il 35° bit impostati a 1.


// Definiamo i pin per il wakeup esterno
#define WAKEUP_PIN_1 GPIO_NUM_33
#define WAKEUP_PIN_2 GPIO_NUM_34


  Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.

  // Configuriamo il wakeup tramite i pin 33 e 34
  // La modalità OR_WAKE indicherà che l'ESP32 si risveglierà se uno dei due pin diventa alto
  esp_sleep_enable_ext1_wakeup((1ULL << WAKEUP_PIN_1) | (1ULL << WAKEUP_PIN_2), ESP_EXT1_WAKEUP_ANY_HIGH);

*/

// case ESP_SLEEP_WAKEUP_TIMER : 
/*
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

*/      



