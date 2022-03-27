
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

volatile int *KEY_ptr = (int *) KEY_BASE;
volatile int *HEX30_ptr = (int *) HEX3_HEX0_BASE;
volatile int *HEX54_ptr = (int *) HEX5_HEX4_BASE;

unsigned int HEX30 = 0;
unsigned int HEX54 = 0;

int main(void){
    *(KEY_ptr + 2) = 0b1111;
    NIOS2_WRITE_IENABLE(0x2);
    NIOS2_WRITE_STATUS(1);

    while (1) {
    }
}