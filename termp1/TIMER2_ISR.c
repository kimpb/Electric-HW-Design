#include "address_map_nios2.h"

#include "nios2_ctrl_reg_macros.h"
extern int cntvalue1;
extern volatile int *TIMER2_ptr;
extern volatile int *TIMER_ptr;
extern int level;
extern volatile int *PS2_ptr;


void TIMER2_ISR(void){
    *TIMER2_ptr = 0;
    if(*TIMER2_ptr & 0b10){


    }

    return;
}