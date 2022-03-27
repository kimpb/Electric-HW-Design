#include "address_map_nios2.h"

extern volatile int *KEY_ptr;
extern volatile int *HEX30_ptr;
extern volatile int *HEX54_ptr;
extern unsigned int HEX30;
extern unsigned int HEX54;

void pushbutton_ISR(void){
    int press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;
    HEX30 = *(HEX30_ptr);
    HEX54 = *(HEX54_ptr);
    switch(press){
    case 0b0001:
        if( (HEX30 & 0x000000FF) != 0 ){
            HEX30 &= 0xFFFFFF00;
        }
        else{
            HEX30 |= 0x0000003F;
        }
        break;
    case 0b0010:
        if( (HEX30 & 0x0000FF00) != 0 ){
            HEX30 &= 0xFFFF00FF;
        }
        else{
            HEX30 |= 0x00000600;
        }
        break;
    case 0b0100:
        if( (HEX30 & 0x00FF0000) != 0 ){
            HEX30 &= 0xFF00FFFF;
        }
        else{
            HEX30 |= 0x005B0000;
        }
        break;
    case 0b1000:
        if( (HEX30 & 0xFF000000) != 0 ){
            HEX30 &= 0x00FFFFFF;
        }
        else{
            HEX30 |= 0x4F000000;
        }
        break;
    }
        *HEX30_ptr = HEX30;
        *HEX54_ptr = HEX54;
    return;
}