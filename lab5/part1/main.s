/*-----------------------------------------
                 main.s
------------------------------------------*/

        .include "address_map_nios2.s"
        .text
        .global _start

_start:
        movia sp, SDRAM_END # stack pointer 초기화
        movia r21, HEX3_HEX0_BASE # r21에 HEX3-0 base address 저장
        stwio r0, 0(r21) # HEX3-0 초기화
        
        movia r10, 0x000000FF
        movia r11, 0x0000FF00
        movia r12, 0x00FF0000
        movia r13, 0xFF000000 # 각 HEX0-3이 ON/OFF인지 판별하기 위한 register r10-r13

        movia r22, KEY_BASE # r22에 pushbutton의 base address 저장
        movi r23, 0b1111 
        stwio r23, 8(r22) # KEY_BASE의 Interruptmask register에 0b1111 저장하여 KEY3-0을 통한 Interrupt 활성화

        movi r23, 2
        wrctl ienable, r23 # interrupt enable bit에 0b10을 write하여 pushbutton을 통한 interrupt 활성화
        movi r23, 1
        wrctl status, r23 # turn on NiosII interrupt processing

IDLE:   
        br IDLE
        .end