# Arduino_RTOS
An RTOS I wrote for arduino or even the atmega328p. 

It is a Round Robin scheduler. 


Uses Timer2. Is really inefficient and cannot be used with Arduino IDE. 

Uses too much implicit Pointer to Int conversion C++ does not like that. 


Was planning on using AVR-GCC to compile with make. But got lazy and wrote it in Arduino IDE. Still written all in C. 

## Documentation
### Blinking lights example 

```C
#include "RTOS.h"

void task1(struct Task * self){ // task 1 blinks pin 2
  while(1){
    PORTD ^= 4;
    _delay_ms(50);
  }
}

void task2(struct Task * self){ // task 2 blinks pin 3
  while(1){
    PORTD ^= 8;
    _delay_ms(200);
  }
}



int main() {
  DDRD = 0b00001100;
  RTOS_INIT // starts the RTOS config stuff 
      queue_task(&task1,1);
      queue_task(&task2,1);
  RTOS_START //Begins the RTOS

  while(1){
    // you could maybe do stuff here
  };
}
```



### Functions
```C
void post(volatile struct Semaphore * s); // Posts a Sempahore
void pend(volatile struct Semaphore * s); // waits on a Sempahore
void lock(volatile struct Mutex * s); // locks a Mutex
void unlock(volatile struct Mutex * s); // unlocks a Mutex
int queue_task(void (*task)(struct Task * self), int priority);
// queue_task takes a pointer to a function which returns nothing that takes a struct Task* as a argument
//Also takes an int that defines it's priority. Higher the priority higher the ammount of time this task is given by the kernel. 
//Is used to queue up tasks


//other stuff


// this was all meant for debug but eventually made it into the final product

void segfault(); // meant to be a catch all for memory violations currently unused

void Henry_Print(const char * d); // Like serial print, you could write your own print function. The name is an inside Joke.


void int_strp(int f);
/* This one stands for int to string print and it prints all leading 0's and will get
the number printed to a 10 digit decimal accuracy. 
*/
```

### Race conditons???
There's mutexes and sempahores. Not going to go indepth into how they work. 

```C
// mutex example
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

int main() {
  DDRD = 0b00001100;
  RTOS_INIT // starts the RTOS config stuff 
      queue_task(&task1,1);
      queue_task(&task2,1);
  RTOS_START //Begins the RTOS

  while(1){
    // you could maybe do stuff here
  };
}

```



```C
// Semaphore example
#include "RTOS.h"
struct Semaphore example;

int task1(struct Task * self){
  pend(&example); // lock wait for data to be avalible // spinlocks it if you will
  //does stuff with resource that was protected
  post(&example); // unlocks data for other task to access
}


int task2(struct Task * self){
  pend(&example); // lock wait for data to be avalible // spinlocks it if you will
  //does stuff with resource that was protected
  post(&example); // unlocks data for other task to access
}

int main() {
  DDRD = 0b00001100;
  RTOS_INIT // starts the RTOS config stuff 
      queue_task(&task1,1);
      queue_task(&task2,1);
  RTOS_START //Begins the RTOS

  while(1){
    // you could maybe do stuff here
  };
}

```

Since the ATMEGA328p is a single core there are no lock instructions and there 
cannot be any race conditions while making changes to a mutex condition or semaphore condition.
Semaphores and Mutexes are still needed to protect memory from diffrent tasks because of the nature of round robin scheduling. 


### Important stuff to know
- You're only allowed 5 tasks. This can be changed by changing the MAX_TASK define

### Extra Knowledge
- 9600 baud is the default for USART
- USART_SPINLOCK is a sham and was a quick solution 
- I wrote instead of checking the ATMEGA328p datasheet for the correct bit to check
- PSP and PBP print out the stack pointer register and the Y register respectivly the Y register is like the base pointer in x86 to AVR GCC
- #define maximum_processor_stack_size 100 You can redfine this if you want a bigger stack size for each task



