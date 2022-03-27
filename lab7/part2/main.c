
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile unsigned int *KEY_ptr = (int *) KEY_BASE;
volatile unsigned int *TIMER_ptr = (int *) TIMER_BASE;
volatile unsigned int *LEDR_ptr = (int *) LEDR_BASE;

unsigned int countervalue;
unsigned int count = 0;

int main(void){
    *(KEY_ptr + 2) = 0b1001;
    countervalue = 25000000;
    *(TIMER_ptr + 1) = 0b1011;
    *(TIMER_ptr + 2) = countervalue & 0xFFFF;
    *(TIMER_ptr + 3) = (countervalue >> 16) & 0xFFFF;
    NIOS2_WRITE_IENABLE(0b11);
    NIOS2_WRITE_STATUS(1);

    while (1) {
    }
}