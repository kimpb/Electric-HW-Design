
/*-----------------------------------------
                 key_isr.s
------------------------------------------*/

        .include "address_map_nios2.s"
        .global ON_KEY_PRESS # global subroutine 선언
/* key3-0을 누르면 interrupt가 발생하여 call 되는 subroutine ON_KEY_PRESS */
ON_KEY_PRESS:
                subi sp, sp, 12 # stack pointer 공간 확보
                stw r22, 0(sp)
                stw r23, 4(sp)
                stw ra, 8(sp) # r22, r23, return address stack에 저장

                movia r22, KEY_BASE # r22에 KEY3-0의 base address 저장
                ldwio r23, 0xC(r22) 
                stwio r23, 0xC(r22) # r23에 pushbutton edge capture register값 저장 후 초기화
                andi r16, r23, 0x1
                bne r16, r0, RESET # 누른 button이 key0면 RESET, 아니면 counter 작동 시작
                ldwio r17, 0(r20) # r20 = TIMER BASE, r17에 interval timer register의 status register값 저장
                andi r17, r17, 0b10 
                beq r23, r0, KEY_PRESS_END # pushbutton edge capture register값이 0이면, 종료
                bne r17, r0, STOP # pushbutton을 눌렀을 때 interval timer register의 RUN이 0이 아니면, br STOP
                movi r12, 0b0111 # RUN이 0이 아니면,
                stbio r12, 4(r20) # interval timer의 control register에 0b0111을 저장하여 timer가 동작하도록 함
                br KEY_PRESS_END
STOP:           movi r12, 0b1011 # pushbutton을 눌렀을 때 RUN이 1이면,
                stbio r12, 4(r20) # interval timer의 control register에 0b1011을 저장하여 timer가 멈추도록 함
                br KEY_PRESS_END
RESET:          mov r3, r0 # count값 초기화
                movi r12, 0b1011 
                stbio r12, 4(r20) # interval timer 작동 멈춤
                stwio r0, 0(r21) # r21 = LEDR_BASE, LEDR 모두 OFF


KEY_PRESS_END:  ldw r22, 0(sp)
                ldw r23, 4(sp)
                ldw ra, 8(sp)
                addi sp, sp, 12 # r22, r23, ra, stack pointer 복구하여 return
                ret                

