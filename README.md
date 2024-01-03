# Arduino_RTOS
An RTOS I wrote for arduino or even the atmega328p. 


Uses Timer2. Is really inefficient. 


Was planning on using AVR-GCC to compile with make. But got lazy and wrote it in Arduino IDE. Still written all in C. 

## Documentation
### Race conditons???
There's mutexes and sempahores. Not going to go indepth into how they work. 

```C
#include "RTOS.h"
struct Mutex example;

int task1(struct Task * self){
  lock(&example); // lock wait for data to be avalible // spinlocks it if you will
  example.data = "Hello world"; // data is always a void pointer in this case it's now pointing to a const char * which is pointing to "Hello World"
  unlock(&example); // unlocks data for other task to access
}


int task2(struct Task * self){
  lock(&example); // lock wait for data to be avalible // spinlocks it if you will
  Henry_Print(example.data); // uses the inbuilt debgugger print
  unlock(&example); // unlocks data for other task to access
}

int main(){
  //sets up RTOS and queues up tasks 
} 

```




