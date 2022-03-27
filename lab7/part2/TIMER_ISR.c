#include "address_map_nios2.h"

extern volatile int *TIMER_ptr;
extern unsigned int count;
extern volatile int *LEDR_ptr;

void TIMER_ISR(void){
    *TIMER_ptr = 0;
    if(*TIMER_ptr & 0b10)
    count++;
    if(count > 0b1111111111){
    *(TIMER_ptr + 1) = 0b1011;
    count = 0;
    }
    *LEDR_ptr = count;
    return;
}