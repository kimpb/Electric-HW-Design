
/*-----------------------------------------
                 count.s
------------------------------------------*/

        .include "address_map_nios2.s"
        .global COUNT # global subroutine 선언
/* interval timer가 작동 중일 때 1초마다 HW interrupt가 발생하여 call 되어서 led를 통해 COUNT하는
Subroutine COUNT */
COUNT:  
                subi sp, sp, 12 # stack pointer 공간 확보
                stw r22, 0(sp)
                stw r23, 4(sp)
                stw ra, 8(sp) # r22, r23, ra 스택에 저장
             
              
                movia r20, TIMER_BASE
                stwio r0, 0(r20) # interval timer의 TO 초기화
                addi r3, r3, 1 # count값 누적
                blt r3, r4, SKIP # r3가 LEDR을 통해 표현할 수 있는 최대값 r4(0b1111111111)보다 작으면, SKIP
                mov r3, r0 # r3가 r4보다 크면, r3는 0으로 초기화
SKIP:           stwio r3, 0(r21) # r21 = LEDR_BASE, r3값을 LEDR에 출력

                ldw r22, 0(sp)
                ldw r23, 4(sp)
                ldw ra, 8(sp)
                addi sp, sp, 12 # r22, r23, return address, stack pointer 원상복구 후 return
                ret            

