#include "RTOS.h"
void post(volatile struct Semaphore * s){
  s->counter=0;
}
void pend (volatile struct Semaphore * s){
  while(s->counter != 0)TIMER2_COMPA_vect();
  s->counter = 1;
}

void lock(volatile struct Mutex * m){
  m->counter=0;
}
void unlock (volatile struct Mutex * m){
  while(m->counter == 1)TIMER2_COMPA_vect();
    m->counter = 1;
}



int queue_task(void (*task)(struct Task * self), int priority){
    struct Task buffer = {
      .running = 0,
      .priority = priority,
      .stack={},
      .SP_=0,
      .task = task
    };
    queue.queue[queue.tasks] = buffer;
    queue.tasks++;
    queue.tasks != 0 ? 
        queue.tasks%=MAX_TASK :
        "";
}



 
void int_strp(int f){ // Integer print

  int buf =f;
  
  char k[11] = "";
  
  for(int i =0; i< 11; i++){
     k[10-i] =(buf%10)^48;
     if(buf !=0) buf = (int) (buf/10);
  }
  
  for(int i =0; i< 11; i++){
    UDR0= k[i];
    USART_SPINLOCK;
  }
  
  UDR0 = '\n';
  USART_SPINLOCK;
}


void Henry_Print(const char * d){ // Serial Print

  for(int i=0; d[i] !=0; i++){
    UDR0 = d[i];
    USART_SPINLOCK;
  }
  USART_SPINLOCK;
}

void segfault(){
  cli();
  Henry_Print("Segfault");
  while(1);
}


 ISR(TIMER2_COMPA_vect){
  cli();
    if(queue.queue[queue.current_task_running].running ==1){
          queue.queue[queue.current_task_running].SP_ = SP;          
          queue.queue[queue.current_task_running].running = 2;
          SP = &queue.queue[queue.current_task_running].stack[maximum_processor_stack_size-2]-2;
          __asm__ volatile("RET"); 
          // Jumps to a position where the task is pushed aside and finished
          // This wastes approximately the time it takes to perform a 
    }

    Queue_Next_Task_After_Incomplete_Task: "";
    // fastest and cleanest way to implement this maybe
    
    
    volatile struct Task * cur_task = &queue.queue[queue.current_task_running];
    //Henry_Print("Task Running: ");
    //int_strp(queue.current_task_running);

    //int_strp(current_sp);
    //int_strp( &queue.queue[queue.current_task_running].stack[0]);

    if (cur_task->priority >= 1){
      
      OCR1A = cur_task->priority*3000;
      
      current_sp = SP; // outside stack save

      if(cur_task->running != 2){ // first run unrun task
        cur_task->running = 1;
        SP = &cur_task->stack[maximum_processor_stack_size-2];
        TCNT2 = 0; 
        sei();
        cur_task->task(cur_task);
        /*Be warned code after this point did not have context saved 
        and if returning from a task that did not finish will not 
        retain cur_task variable or any other variable for that fact that 
        isn't static or global
        */
        cur_task = &queue.queue[queue.current_task_running];// the cur_task is being redefined for that reason
      }else if(cur_task->running == 2){ 
        cur_task->running = 1; // context_switch back into previous task
        TCNT2 = 0;  // resets Timer2
        SP=queue.queue[queue.current_task_running].SP_;
        goto ctx_switch; // saves Program Memory
      }
      current_sp = current_sp; // There's a compiler optimization 
      //I don't know the exact name of it but it 
      //clobbers the current_sp variable or SP
      //this fixes it by refreshing current_sp 
      //Setting any varible or moving values above this operation
      // fixes the issues of spinlocks messing up the stack pointer
      SP = (volatile uint16_t)current_sp;

      if(cur_task->running == 1){
        cur_task->priority =-1;
        cur_task->running = 0;
        queue.current_task_running++;
      }else if(cur_task->running == 2){
         queue.current_task_running++;
      }
      
    }else if (cur_task->priority == 0){
      queue.current_task_running = 0;
    }else if (cur_task->priority == -1){
      queue.current_task_running++;
      sei();
    }
    queue.current_task_running %= MAX_TASK;
    ctx_switch: // context switch created by auto interrupt // Saves me program memory but is not good practice...
    "";
}
