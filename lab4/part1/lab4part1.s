

        
        .text
        .global _start
        .include "address_map_nios2.s"

_start:
        movia r7, KEY_BASE
        movia r8, HEX3_HEX0_BASE
        movhi sp, 0x2
        mov r9, r0

MAIN:   
KEY_PRESS_WAIT:
        ldwio r6, 0(r7)
        beq r6, r0, KEY_PRESS_WAIT
KEY_RELEASE_WAIT:
        ldwio r5, 0(r7)
        bne r5, r0, KEY_RELEASE_WAIT
        andi r4, r6, 0x1
        bne r4, r0, RESET_CALL
        srli r6, r6, 0x1
        andi r4, r6, 0x1
        bne r4, r0, INCREMENT_CALL
        srli r6, r6, 0x1
        andi r4, r6, 0x1
        bne r4, r0, DECREMENT_CALL
        srli r6, r6, 0x1
        andi r4, r6, 0x1
        bne r4, r0, OFF_CALL
        br MAIN

INCREMENT_CALL:
        call INCREMENT
        br MAIN
DECREMENT_CALL:
        call DECREMENT
        br MAIN
RESET_CALL:
        call RESET
        br MAIN
OFF_CALL:
        call OFF
        br MAIN

INCREMENT:
        addi r9, r9, 0x1
        subi sp, sp, 0x4
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

        
DECREMENT:
        beq r9, r0, JUMP
        subi r9, r9, 0x1
JUMP:   subi sp, sp, 0x4
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

RESET: 
        mov r9, r0
        mov r4, r0
        subi sp, sp, 0x4
        stw ra, 0(sp)
        call SEG7_CODE
        ldw ra, 0(sp)
        addi sp, sp, 0x4
        stwio r2, 0(r8)
        ret

OFF:
        mov r9, r0
        stwio r0, 0(r8)
        ret





DIVIDE: 
        mov     r2, r4 /* subroutine DIVIDE, r2 <= r4 (r2 = remainder, r4 = dividend) */
        movi    r5, 10 /* r5 <= 10 (r5 = divisor) */
        movi    r3, 0 /* r3 <= 0 (r3 = quotient) */
CONT:   blt     r2, r5, DIV_END /* if r2 < r5, go to DIV_END */
        sub     r2, r2, r5 /* r2 <= r2 - 10 */
        addi    r3, r3, 1 /* r3 <= r3 + 1 */
        br      CONT /* go to CONT */ 
DIV_END:  
        ret /* if r2 < r5, return. quotients is in r3, remainder is in r2 */

SEG7_CODE:
        movia   r15, BIT_CODES # starting address of bit codes
        add     r15, r15, r4 # index into the bit codes
        ldb     r2, (r15) # read the bit code needed for our digit
        ret

BIT_CODES:.byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
        .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
        .skip   2 # pad with 2 bytes to maintain word alignment

        .end
