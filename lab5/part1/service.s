/*-----------------------------------------
                 service.s
------------------------------------------*/

.section	.reset, "ax"
                movia r2, _start
                jmp r2

.section	.exceptions, "ax"

.global     ISR # interrupt service routine ISR

ISR:        subi sp, sp, 16 # stack pointer 공간 확보
            stw et, 0(sp) # exception temporary를 스택에 저장
            rdctl et, ctl4 # ipending을 가져와서 
            beq et, r0, SKIP_EA_DEC # HW interrupt인지 SW interrupt인지 판별
            subi ea, ea, 4 # ea는 SW interrupt일 때만 감소

SKIP_EA_DEC:
            stw ea, 4(sp)
            stw ra, 8(sp)
            stw r22, 12(sp)) # exception return address, return address, r22값들 stack에 저장

            rdctl et, ctl4
            bne et, r0, ON_EXT_INT # ipending을 가져와서 HW interrupt인지 SW interrupt인지 판별

ON_TRAP:    
            br END_ISR # SW interrupt일 경우 종료
ON_EXT_INT: 
            andi r22, et, 0b10 # HW interrupt 일 경우, ipending의 두번째 bit,
            beq r22, r0, END_ISR # IRQ #1(pushbutton switch parallel port)가 0이면 종료,
            call ON_KEY_PRESS # 1이면 ON_KEY_PRESS subroutine을 call한다.

END_ISR:    
            ldw et, 0(sp)
            ldw ea, 4(sp)
            ldw ra, 8(sp)
            ldw r22, 12(sp)
            addi sp, sp, 16 # exception return address, return address, r22, stack pointer 원상복구 
            eret # exception return
            .end

