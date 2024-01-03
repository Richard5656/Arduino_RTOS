# Arduino_RTOS
An RTOS I wrote for arduino or even the atmega328p. 


Uses Timer2. Is really inefficient. 


Was planning on using AVR-GCC to compile with make. But got lazy and wrote it in Arduino IDE. Still written all in C. 

## Documentation
### Functions

```C
void post(volatile struct Semaphore * s); // Posts a Sempahore
void pend(volatile struct Semaphore * s); // waits on a Sempahore
void lock(volatile struct Mutex * s); // locks a Mutex
void unlock(volatile struct Mutex * s); // unlocks a Mutex
int queue_task(void (*task)(struct Task * self), int priority);
// queue_task takes a pointer to a function which returns nothing that takes a struct Task* as a argument
//Is used to queue up tasks


//other stuff


// this was all meant for debug but eventually made it into the final product
void segfault(); // meant to be a catch all for memory violations currently unused
void Henry_Print(const char * d); // Like serial print, you could write your own print function. The name is an inside Joke.
void int_strp(int f); // This one stands for int to string print and it prints all leading 0's and will get the number to a 10 digit decimal accuracy. 


```



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
  Henry_Print(example.data); // uses the inbuilt debgugger print //
  unlock(&example); // unlocks data for other task to access
}

int main(){
  //sets up RTOS and queues up tasks 
} 

```




