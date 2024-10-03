#ifndef CALIBRATION_H
#define CALIBRATION_H

#define TASK_CALIB   100                // 0.1ms

#include <Arduino.h>

typedef struct {
    String nome;
    int valore;
} Variabile;

extern Variabile variabili[4];

#ifdef ENABLE_CALIBRATION
  void gestisciSeriale();
  void task_calib(void* pvParameters);
#endif


#endif
