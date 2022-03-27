#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern volatile int *TIMER_ptr;
extern volatile int *TIMER2_ptr;
extern unsigned int count;
extern unsigned int cntvalue1;

void pushbutton_ISR(void){
    int press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;
    switch(press){
    case 0b1000:
        if(*TIMER_ptr & 0b10){
        *(TIMER_ptr + 1) = 0b1011;
        *(TIMER2_ptr + 1) = 0b1011;
        }
        else{
        *(TIMER_ptr + 1) = 0b0111;
        *(TIMER2_ptr + 1) = 0b0111;
        video_text(55, 30, "                        \0");
        }
        break;
    case 0b0100:
        if(cntvalue1 > 0x8FFFFFFF)
        cntvalue1 = 25000000;
        else{
            cntvalue1 *= 2;
            *(TIMER_ptr + 1) = 0b1011;
            *(TIMER_ptr + 2) = cntvalue1 & 0xFFFF;
            *(TIMER_ptr + 3) = (cntvalue1 >> 16) & 0xFFFF;
            *(TIMER_ptr + 1) = 0b0111;
        }
        break;
    case 0b0010:
        if(cntvalue1 == 1)
        cntvalue1 = 25000000;
        else{
            cntvalue1 /= 2;
            *(TIMER_ptr + 1) = 0b1011;
            *(TIMER_ptr + 2) = cntvalue1 & 0xFFFF;
            *(TIMER_ptr + 3) = (cntvalue1 >> 16) & 0xFFFF;
            *(TIMER_ptr + 1) = 0b0111;
        }
        break;
    case 0b0001:
        *(TIMER_ptr + 1) = 0b1011;
        count = 0;
        break;
    }
    return;
}