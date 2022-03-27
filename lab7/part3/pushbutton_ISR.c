#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern unsigned int count;
extern unsigned int countervalue;
extern volatile int *LEDR_ptr;

void pushbutton_ISR(void){
    int press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;
    switch(press){
    case 0b1000:
        if(*TIMER_ptr & 0b10)
        *(TIMER_ptr + 1) = 0b1011;
        else
        *(TIMER_ptr + 1) = 0b0111;
        break;
    case 0b0100:
        if(countervalue > 0x8FFFFFFF)
        countervalue = 25000000;
        else{
            countervalue *= 2;
            *(TIMER_ptr + 1) = 0b1011;
            *(TIMER_ptr + 2) = countervalue & 0xFFFF;
            *(TIMER_ptr + 3) = (countervalue >> 16) & 0xFFFF;
            *(TIMER_ptr + 1) = 0b0111;
        }
        break;
    case 0b0010:
        if(countervalue == 1)
        countervalue = 25000000;
        else{
            countervalue /= 2;
            *(TIMER_ptr + 1) = 0b1011;
            *(TIMER_ptr + 2) = countervalue & 0xFFFF;
            *(TIMER_ptr + 3) = (countervalue >> 16) & 0xFFFF;
            *(TIMER_ptr + 1) = 0b0111;
        }
        break;
    case 0b0001:
        *(TIMER_ptr + 1) = 0b1011;
        count = 0;
        *LEDR_ptr = count;
        break;
    }
    return;
}