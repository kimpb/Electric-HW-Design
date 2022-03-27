        
        .text
        .global _start
        .include "address_map_nios2.s"

_start: 
        movia r7, KEY_BASE
        movia r8, HEX3_HEX0_BASE
        movia r19, TIMER_BASE
        movhi sp, 0x2
        movui r20, 0xE100
        stwio r20, 8(r19)
        movi r21, 0x05F5
        stwio r21, 12(r19)
        movi r18, 1000
        mov r9, r0
        call HEX

MAIN:   
        ldwio r16, 0(r19)
        andi r16, r16, 0b10
        beq r16, r0, KEY_PRESS_WAIT
        ldwio r16, 0(r19)
        andi r16, r16, 0b1
        beq r16, r0, KEY_PRESS_WAIT
        addi r11, r11, 0b1
        bge r11, r18, RESET

        mov r9, r11
        call HEX
        stbio r0, 0(r19)

KEY_PRESS_WAIT:
        ldwio r6, 0(r7)
        beq r6, r0, MAIN
KEY_RELEASE_WAIT:
        ldwio r5, 0(r7)
        bne r5, r0, KEY_RELEASE_WAIT
        andi r4, r6, 0x1
        bne r4, r0, COUNT
        srli r6, r6, 0x1
        andi r4, r6, 0x1
        bne r4, r0, RESET
        srli r6, r6, 0x1
        andi r4, r6, 0x1
        bne r4, r0, STOP
        br MAIN


COUNT:  
        movi r14, 0b0110
        sthio r14, 4(r19)
        br MAIN

RESET:  
        mov r11, r0
        movi r14, 0b1010
        sthio r14, 4(r19)
        mov r9, r11
        call HEX
        br MAIN

STOP:
        movi r14, 0b1010
        sthio r14, 4(r19)
        mov r9, r11
        call HEX
        br MAIN




HEX:    subi sp, sp, 0x4
        stw ra, 0(sp)
        mov r4, r9
        call DIVIDE
        mov r12, r2
        mov r4, r3
        call DIVIDE
        mov r13, r2
        mov r4, r12
        call SEG7_CODE
        mov r12, r2
        mov r4, r13
        call SEG7_CODE
        mov r13, r2
        mov r4, r3
        call SEG7_CODE
        slli r2, r2, 16
        slli r13, r13, 8
        or r2, r2, r13
        or r2, r2, r12
        ldw ra, 0(sp)
        addi sp, sp, 0x4
        stwio r2, 0(r8)
        ret



DIVIDE: 
        subi sp, sp, 0x4
        stw ra, 0(sp)
        mov     r2, r4 /* subroutine DIVIDE, r2 <= r4 (r2 = remainder, r4 = dividend) */
        movi    r5, 10 /* r5 <= 10 (r5 = divisor) */
        movi    r3, 0 /* r3 <= 0 (r3 = quotient) */
CONT:   blt     r2, r5, DIV_END /* if r2 < r5, go to DIV_END */
        sub     r2, r2, r5 /* r2 <= r2 - 10 */
        addi    r3, r3, 1 /* r3 <= r3 + 1 */
        br      CONT /* go to CONT */ 
DIV_END:ldw     ra, 0(sp)
        addi    sp, sp, 0x4        
        ret /* if r2 < r5, return. quotients is in r3, remainder is in r2 */

SEG7_CODE:
        subi    sp, sp, 0x4
        stw     ra, 0(sp)
        movia   r15, BIT_CODES # starting address of bit codes
        add     r15, r15, r4 # index into the bit codes
        ldb     r2, (r15) # read the bit code needed for our digit
        ldw     ra, 0(sp)
        addi    sp, sp, 0x4
        ret

BIT_CODES:.byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
        .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
        .skip   2 # pad with 2 bytes to maintain word alignment

        .end

