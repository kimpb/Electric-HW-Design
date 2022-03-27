#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include <stdlib.h>

#define LEFT 0x6B
#define RIGHT 0x74
#define DOWN 0x72
#define UP 0x75
#define SPACE 0x29
#define ESC 0x76
#define true 1
#define false 0
#define RO 100

extern volatile int *PS2_ptr;
extern volatile int *TIMER_ptr;
extern int dx, dy, rotation, crush_on;
extern int byte1, byte2, byte3;

void ps2_ISR(){
    int PS2_data, RVALID;

    PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    RVALID = PS2_data & 0x8000; // extract the RVALID field

    if(RVALID){
    byte1 = byte2;
    byte2 = byte3;
    byte3 = PS2_data & 0xFF;
    if(byte2 == 0xF0){
    switch(byte3){
        case ESC:
        reset();
        break;
        case LEFT:
            if(check_crush(dx-1, dy, rotation) == true)
            move_block(LEFT);
            break;
        case RIGHT:
            if(check_crush(dx+1, dy, rotation) == true)
            move_block(RIGHT);
            break;
        case DOWN:
            if(check_crush(dx, dy+1, rotation) == true)
            move_block(DOWN);
            break;
        case UP:
            if(check_crush(dx, dy, (rotation+1)%4) == true)
            move_block(UP);
            else if(check_crush(dx, dy-1, (rotation+1)%4) == true)
            move_block(RO);
            break;
        case SPACE:
                drop_block();
            break;
    }
    }
    }
}