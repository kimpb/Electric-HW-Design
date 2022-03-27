
#include <stdio.h>
#include "address_map_nios2.h"

static volatile int *HEX3_HEX0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_HEX4_ptr = (int*) HEX5_HEX4_BASE;
static volatile int *KEY_ptr = (int*) KEY_BASE;
static volatile int *TIMER_ptr = (int*) TIMER_BASE;

int SEG7_CODE(int i){
    char BIT_CODE[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};
        return BIT_CODE[i];
}

void display_HEX5_0(unsigned int a){
    unsigned int min, sec, ms10, b, c;
    min = a / 60000;
    sec = (a % 60000) / 1000;
    ms10 = (a % 1000) / 10;
    
    c = SEG7_CODE(min/10);
    b = SEG7_CODE(min%10);

    c = (c<<8) | b;
    *HEX5_HEX4_ptr = c;

    c = SEG7_CODE(sec/10);
    b = SEG7_CODE(sec%10);
    c = (c<<8) | b;
    b = SEG7_CODE(ms10/10);
    c = (c<<8) | b;
    b = SEG7_CODE(ms10%10);
    c = (c<<8) | b;
    *HEX3_HEX0_ptr = c;

}

void Timercontrol(int a){
    int *TIMER_control = (int*)(TIMER_BASE+4);
    *TIMER_control = a;
}

void KEY_PRESS(){
    if(*TIMER_ptr)
    Timercontrol(0b1010);
    else
    Timercontrol(0b0110);
}


int main(){
    int count = 0;
    int *TIMER_periodl = (int*)(TIMER_BASE+8);
    int *TIMER_periodh = (int*)(TIMER_BASE+12);

    *TIMER_periodl = 100000;
    *TIMER_periodh = 100000 >> 16;

    while(1){
        if(*(TIMER_ptr) & 0b1 != 0){
            count++;
            if(count >=60000*100){
                count = 0;
                Timercontrol(0b1010);
            }
            display_HEX5_0(count);
            *TIMER_ptr = 0;
        }
            while(*KEY_ptr){
                while(*KEY_ptr){
                }
                KEY_PRESS();
            }
        
    }
}