#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#define MAX_TASK 5
#define F_CPU 4000000UL  // supposed to be 16000000UL for 16 meghagertz but 4 mHz made the delay go faster.


#define BAUDRATE (((16000000UL / (9600 * 16UL))) - 1)
#include <util/delay.h>

#define RTOS_INIT  cli();\
      UBRR0H = (BAUDRATE >> 8); \
      UBRR0L = BAUDRATE; \
      UCSR0B |= (1 << TXEN0) | (1 << RXEN0); \
      UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); \ 
      TCCR2A |= 0;  \        
      TCCR2B = 0;           \
      TCCR2B |= 0b00000010;  \
      OCR2A =255;        \
      TIMSK2 |= 0b00000010;  \

#define RTOS_START sei();

#pragma GCC optimize ("-O0") 
// these compiler optimizations are clobbering registers and other values

 ISR(TIMER2_COMPA_vect);
#define maximum_processor_stack_size 100 
#define PSP Henry_Print("SP:" );int_strp(SP); 
#define PBP    volatile uint16_t Y; \
    __asm__("MOVW %0,R28" : "=r" (Y)); \
    Henry_Print("BP: "); \
    int_strp(Y);

#define _Henry_Delay(timeMS) // will possibly not be implmented neess timer0
#define USART_SPINLOCK for(volatile int d_spinlock_usart=0; d_spinlock_usart < 1000; d_spinlock_usart++); // an arbotrary ammount of time wasted by cycles to use USART 
//because I'm too lazy to implement a better delay function



struct Task{
  uint8_t running:4; // 0: not running 1 : running 2: waiting to run again
  int8_t priority;
  volatile uint8_t stack[maximum_processor_stack_size]; // this is the part of the stack that the return address is on
  volatile uint16_t SP_;
  void (*task)(struct Task * self);
};
struct Queue{
  struct Task queue[MAX_TASK-1];
  volatile uint8_t tasks;
  unsigned long int current_sp;
  uint16_t interrupt_nest;
  uint8_t current_task_running; // is a relative pointer to whatever is in the queuee PID 

};

volatile uint16_t current_sp;
volatile struct Queue queue; // will be the main queue (Should make a macro to initialize this)



struct Semaphore {
  uint8_t counter;
};

struct Mutex{
  uint8_t counter;
  void * data;
};

void post(volatile struct Semaphore * s);
void pend(volatile struct Semaphore * s);
void lock(volatile struct Mutex * s);
void unlock(volatile struct Mutex * s);
int queue_task(void (*task)(struct Task * self), int priority);
void Henry_Print(const char * d);
void int_strp(int f);
void segfault();
