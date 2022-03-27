        .text
        .global _start
ZEROS:  nor     r8, r4, r0
        mov     r2, r0
        br      LOOP

ONES:   
        mov     r8, r4
        mov     r2, r0
        br      LOOP

ALTERNATE:
        mov     r14, ra
        mov     r7, r0
        mov     r2, r0
        movhi   r7, 0x5555
        ori     r7, r7, 0x5555
        xor     r8, r4, r7
        call    LOOP
        mov     r6, r2
        mov     r2, r0
        srli    r7, r7, 0x01
        xor     r8, r4, r7
        call    LOOP
        bge     r2, r6, ALT_END
        mov     r2, r6
ALT_END:mov     ra, r14
        ret


LOOP:   beq     r8, r0, LOOP_END /* r4 = 0이면, subroutine 종료 */
        srli    r9, r8, 0x01 /* r5 <= r4 >> 1 */
        and     r8, r8, r9 /* r4 <= r4 & r5 하여 1이 연속되어 나오는 부분만 살림 */
        addi    r2, r2, 0x01 /* r2 = r10 + 1 하여 연속된 1의 개수 count */
        br      LOOP /* ONES로 가서 반복 */
LOOP_END:ret      /* return */

_start: ldw r4, TEST_NUM(r0)
        mov r10, r0
        mov r11, r0
        mov r12, r0
        call ZEROS
        mov r10, r2 /*r10 = count of the longest 0’s*/
        call ONES
        mov r11, r2 /*r11 = count of the longest 0’s*/
        call ALTERNATE
        mov r12, r2 /*r12 = count of the longest 10’s*/
END:    br END
TEST_NUM:.word 0xFF555A00 /*an example*/
        .end        