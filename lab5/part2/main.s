/*-----------------------------------------
                 main.s
------------------------------------------*/

        .include "address_map_nios2.s"
        .text
        .global _start

_start:
        movia sp, SDRAM_END # sp에 SDRAM_END값 저장
        mov r3, r0 # count를 하는 r3값 초기화
        movia r20, TIMER_BASE # r20에 TIMER_BASE의 base address 저장
        movia r12, 100000000 # 10ns * 10^8 = 1s, 10^8 = 100,000,000
        sthio r12, 8(r20) # interval timer의 counter start value(low) <= 0xE100 저장
        srli r12, r12, 16
        sthio r12, 12(r20) # interval timer의 counter start value(high) <= 0x05F5 저장
        movi r12, 0b1011
        sthio r12, 4(r20) # interval timer의 control register값 0b1011로 초기화하여 counter 비활성 상태로 초기화

        movia r21, LEDR_BASE # r21에 TIMER_BASE의 base address 저장
        movi r4, 0b1111111111 # LEDR을 통해 표현가능한 최대값을 register r4 <= 0b1111111111 (LEDR9-0)에 저장
        stwio r0, 0(r21) # LEDR 초기화
        
        

        movia r22, KEY_BASE # r22에 KEY_BASE의 base address 저장
        movi r23, 0b1111
        stwio r23, 8(r22) # KEY_BASE의 Interruptmask register에 0b1111 저장하여 KEY3-0을 통한 Interrupt 활성화

        movi r23, 0b11
        wrctl ienable, r23 # interrupt enable bit에 0b11을 write하여 interval timer와 pushbutton을 통한 interrupt 활성화
        movi r23, 1
        wrctl status, r23 # set processor enable bit ON

IDLE:   
        br IDLE
        .end
