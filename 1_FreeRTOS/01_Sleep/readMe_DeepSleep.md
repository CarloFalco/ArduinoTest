# Datatype:
*esp_sleep_wakeup_cause_t wakeUpRsn*;  
*wakeUpRsn = esp_sleep_get_wakeup_cause()*; Sleep wakeup cause.



# modalità di deep sleep:

1. ESP_SLEEP_WAKEUP_EXT0: Wakeup caused by external signal using RTC_IO.
1. ESP_SLEEP_WAKEUP_EXT1: Wakeup caused by external signal using RTC_CNTL.
1. ESP_SLEEP_WAKEUP_TIMER: Wakeup caused by timer.




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
so: 0x0000000600000000 it's mean both bit 33 and 34 = 1


**mode**: Mode: the logic to wake up the ESP32. It can be:
- ESP_EXT1_WAKEUP_ALL_LOW/HIGH: wake up when all GPIOs go low/high;
- ESP_EXT1_WAKEUP_ANY_LOW/HIGH: wake up if any of the GPIOs go low/high.

*WAKEUP_PIN_1 = GPIO_NUM_33 e WAKEUP_PIN_2 = GPIO_NUM_34*  

*pinMode(WAKEUP_PIN_1, INPUT_PULLDOWN);*  
*pinMode(WAKEUP_PIN_2, INPUT_PULLDOWN);*  

*esp_sleep_enable_ext0_wakeup((1ULL << WAKEUP_PIN_1) | (1ULL << WAKEUP_PIN_2), ESP_EXT1_WAKEUP_ANY_HIGH);* 

*esp_deep_sleep_start();


### ESP_SLEEP_WAKEUP_TIMER
*esp_sleep_enable_timer_wakeup(time_in_us);*

**time_in_us** time before wakeup, in microseconds

*esp_deep_sleep_start();*  


For more information see the official guide available [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html):

