#include "RTOS.h"


void task1(struct Task * self){
  while(1){
    PORTD ^= 4;
    _delay_ms(50);
  }
}
void task2(struct Task * self){
  while(1){
    PORTD ^= 8;
    _delay_ms(200);
  }
}



int main() {
  // put your setup code here, to run once:
  DDRD = 0b11111111;
RTOS_INIT
    queue_task(&task1,1);
    queue_task(&task2,1);
RTOS_START
    while(1){
   // PORTD ^= 31;
 // del(1000);
Henry_Print("he");
    };
}
