#include "count.h"



Count::Count(int num){
  this->num = num;
}


void Count::increase(){
  num++;

}
void Count::decrease(){
  num--;
}

int Count::numVal(){
  return num;
}