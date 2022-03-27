/*******************************************************************************
exception_handler.c
*******************************************************************************/
#include "nios2_ctrl_reg_macros.h"
/* function prototypes */
void main(void);
void interrupt_handler(void);
void pushbutton_ISR(void);
void TIMER_ISR(void);
/* CPU reset processing을 다루는 assembly language code */
void the_reset (void) __attribute__ ((section (".reset")));
void the_reset (void)
/******************************************************************************
* Reset code. ".reset"이라는 section attribute를 사용함으로써 Linker가 이 코드를 
* reset vector address에 위치하도록 함. 이 코드는 main 프로그램을 호출한다.
******************************************************************************/
{
asm (".set noat");  // magic, for the C compiler
asm (".set nobreak");  // magic, for the C compiler
asm ("movia r2, main");  // C 언어의 main program이 있는 주소를 불러옴
asm ("jmp r2"); // Jump
}
/* CPU exception processing을 다루는 assembly language code. 수정 불가.
   대신, C언어로된 interrupt_handler() 는 주어진 application의 필요에 따라 수정될 수 있음.*/
void the_exception (void) __attribute__ ((section (".exceptions")));
void the_exception (void)
/*******************************************************************************
* Exceptions code. ".exceptions"이라는 section attribute를 사용하여 Linker가 이 코드를 
* 이전의 exceptions vector address에 위치하도록 함. 이 코드는 interrupt handler를 호출하고
* Exception으로부터 리턴.
******************************************************************************/
{
asm (".set noat");   // magic, for the C compiler
asm (".set nobreak");  // magic, for the C compiler
asm ( "subi sp, sp, 128");
asm ( "stw et, 96(sp)");
asm ( "rdctl et, ctl4");
asm ( "beq et, r0, SKIP_EA_DEC"); // 외부 interrupt가 아닌 경우 
asm ( "subi ea, ea, 4");  // 외부 interrupt는 리턴 할 주소를 한 instruction 앞당겨서 return
asm ( "SKIP_EA_DEC:" );
asm ( "stw r1, 4(sp)" );  // save all registers
asm ( "stw r2, 8(sp)" );
asm ( "stw r3, 12(sp)" );
asm ( "stw r4, 16(sp)" );
asm ( "stw r5, 20(sp)" );
asm ( "stw r6, 24(sp)" );
asm ( "stw r7, 28(sp)" );
asm ( "stw r8, 32(sp)" );
asm ( "stw r9, 36(sp)" );
asm ( "stw r10, 40(sp)" );
asm ( "stw r11, 44(sp)" );
asm ( "stw r12, 48(sp)" );
asm ( "stw r13, 52(sp)" );
asm ( "stw r14, 56(sp)" );
asm ( "stw r15, 60(sp)" );
asm ( "stw r16, 64(sp)" );
asm ( "stw r17, 68(sp)" );
asm ( "stw r18, 72(sp)" );
asm ( "stw r19, 76(sp)" );
asm ( "stw r20, 80(sp)" );
asm ( "stw r21, 84(sp)" );
asm ( "stw r22, 88(sp)" );
asm ( "stw r23, 92(sp)" );
asm ( "stw r24, 96(sp)" );
asm ( "stw r25, 100(sp)" );  // r25 = bt
asm ( "stw r26, 104(sp)" );  // r26 = gp
                          // r27 = sp, 앞에서 저장했으므로 skip
asm ( "stw r28, 112(sp)" );  // r28 = fp
asm ( "stw r29, 116(sp)" );  // r29 = ea
asm ( "stw r30, 120(sp)" );  // r30 = ba
asm ( "stw r31, 124(sp)" );  // r31 = ra
asm ( "addi fp, sp, 128" );
asm ( "call interrupt_handler" );  // C언어로된 interrupt handler를 호출한다.
asm ( "ldw r1, 4(sp)" );  // restore all registers
asm ( "ldw r2, 8(sp)" );
asm ( "ldw r3, 12(sp)" );
asm ( "ldw r4, 16(sp)" );
asm ( "ldw r5, 20(sp)" );
asm ( "ldw r6, 24(sp)" );
asm ( "ldw r7, 28(sp)" );
asm ( "ldw r8, 32(sp)" );
asm ( "ldw r9, 36(sp)" );
asm ( "ldw r10, 40(sp)" );
asm ( "ldw r11, 44(sp)" );
asm ( "ldw r12, 48(sp)" );
asm ( "ldw r13, 52(sp)" );
asm ( "ldw r14, 56(sp)" );
asm ( "ldw r15, 60(sp)" );
asm ( "ldw r16, 64(sp)" );
asm ( "ldw r17, 68(sp)" );
asm ( "ldw r18, 72(sp)" );
asm ( "ldw r19, 76(sp)" );
asm ( "ldw r20, 80(sp)" );
asm ( "ldw r21, 84(sp)" );
asm ( "ldw r22, 88(sp)" );
asm ( "ldw r23, 92(sp)" );
asm ( "ldw r24, 96(sp)" );
asm ( "ldw r25, 100(sp)" );
asm ( "ldw r26, 104(sp)" ); // r25 = bt
                         // r27 = sp, skip
asm ( "ldw r28, 112(sp)" ); // r28 = fp
asm ( "ldw r29, 116(sp)" ); // r29 = ea
asm ( "ldw r30, 120(sp)" ); // r30 =ba
asm ( "ldw r31, 124(sp)" ); // r31 = ra
asm ( "addi sp, sp, 128" );
asm ( "eret" );
}
/******************************************************************************
* Interrupt Service Routine으로, interrupt의 발생 원인을 정하고 적절한
* subroutine을 호출한다. 
* ipending - pending interrupt bit
*******************************************************************************/
void interrupt_handler(void){
    int ipending;
    NIOS2_READ_IPENDING(ipending); // ipending read
    if(ipending & 0x1) 
    TIMER_ISR( ); // timer에 의한 interrupt면 TIMER_ISR call
    else if(ipending & 0x2)
    pushbutton_ISR( ); // pushbutton에 의한 interrupt면 pushbutton_ISR call
    return;
}

