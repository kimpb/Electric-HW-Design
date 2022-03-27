
        .text
        .global _start
_start: 
        ldw     r9, TEST_NUM(r0) /* r9 <= MEM[TEST_NUM] */

        mov     r10, r0 /* r10 <= 0으로 초기화 */
LOOP:   beq     r9, r0, END /* r9 = 0 이면 END */
        srli    r11, r9, 0x01   /* r11 <= r9 >> 1 */
        and     r9, r9, r11 /* r9 <= r9 & r11 하여 1이 연속되어 나오는 부분만 살림 */
        addi    r10, r10, 0x01 /* r10 = r10 + 1 하여 연속된 1의 개수 count */
        br      LOOP /* go to LOOP */

END:    br      END

TEST_NUM:.word  0x3fabedef
        .end

        