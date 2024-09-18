#ifndef COUNT_H
#define COUNT_H

#include <Arduino.h>

class Count {
  
  private:
    int num;

  public:
  // Led() {} // do not use
  Count(int num);

  void increase();
  void decrease();

  int numVal();

};

#endif