#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern unsigned int count;
extern volatile int *LEDR_ptr;

void pushbutton_ISR(void){
    int press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;
    if(press & 0b1000){
        if(*TIMER_ptr & 0b10)
        *(TIMER_ptr + 1) = 0b1011;
        else
        *(TIMER_ptr + 1) = 0b0111;
    }
    else if(press & 0b0001){
        *(TIMER_ptr + 1) = 0b1011;
        count = 0;
        *LEDR_ptr = count;
    }
    return;
}