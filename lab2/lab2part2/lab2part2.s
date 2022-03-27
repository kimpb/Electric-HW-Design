        .text
        .global _start
ONES:   
        beq     r4, r0, ONES_END /* r4 = 0이면, subroutine 종료 */
        srli    r5, r4, 0x01 /* r5 <= r4 >> 1 */
        and     r4, r4, r5 /* r4 <= r4 & r5 하여 1이 연속되어 나오는 부분만 살림 */
        addi    r2, r2, 0x01 /* r2 = r2 + 1 하여 연속된 1의 개수 count */
        br      ONES /* ONES로 가서 반복 */
ONES_END:ret      /* return */

_start:
        mov     r2, r0 /* count number 초기화 */
        movia   r6, TEST_NUM/* TEST_NUM의 address를 r6에 저장 */
LOOP:   ldw     r4, 0(r6) /* r4로 TEST_NUM 의 address가 나타내는 값 불러들임 */
        beq     r4, r0, END /* 불러들인 수가 0이면, 종료 */
        mov     r2, r0 /* return number 초기화 */
        call    ONES /* subroutine call ONES */
        blt     r2, r10, NEXT /* return value가 r10보다 작으면, go to NEXT */
        mov     r10, r2 /* 그렇지 않으면, r10 <= r2 */
        mov     r9, r6 /* 가장 긴 1의 문자열이 있는 주소 저장 */
NEXT:   addi    r6, r6, 4 /* address of TEST_NUM += 4 */
        br      LOOP /* 그런 뒤, go to LOOP */


END:    br      END

TEST_NUM:.word 0x0000FF00, 0x0000FFA0, 0x1234, 0x5678
        .word  0xABCD, 0xCDEF, 0xFFFF, 0xEFEF
        .word  0xFFFE, 0x000FFFFF, 0x0001FFFF, 0
        /* 가장 긴 연속된 1의 개수 : 20 (FFFFF) */
        .end
        