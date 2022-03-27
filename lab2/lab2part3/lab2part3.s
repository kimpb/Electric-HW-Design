        .text
        .global _start


ZEROS:  nor     r8, r4, r0 # r8에 r4의 inversion값을 저장하기 위해 r0와 r4의 nor연산 후 저장
        mov     r2, r0 # 리턴값 초기화
        br      LOOP # 연속된 1의 개수를 세는 subroutine LOOP call

ONES:   
        mov     r8, r4 # r8에 r4값 저장
        mov     r2, r0 # 리턴값 초기화
        br      LOOP # 연속된 1의 개수를 세는 subroutine LOOP call

ALTERNATE:
        mov     r14, ra # subroutine 내에서 두번의 call 때문에 return address가 변하므로, 다시 돌아갈 return address 저장
        mov     r7, r0 # 연산을 하기 위한 register r7 초기화
        mov     r2, r0 # 리턴 값 초기화
        movhi   r7, 0x5555 # r7 <= 0x55550000
        ori     r7, r7, 0x5555 # r7 <= 0x55555555 (01이 16번 반복되는 32bit 수)
        xor     r8, r4, r7 # 넘겨받은 r4와 r7을 xor하여 10이 반복되면 11으로 치환
        call    LOOP # 연속된 1의 개수를 세는 subroutine LOOP call
        mov     r6, r2 # 10이 반복되는 개수를 r6에 저장
        mov     r2, r0 # 리턴 값 초기화
        movhi   r7, 0xAAAA # r7 <= 0xAAAA0000
        ori     r7, r7, 0xAAAA # r7 <= 0xAAAAAAAA (10이 16번 반복되는 32bit 수)
        xor     r8, r4, r7 # 넘겨받은 r4와 r7을 xor하여 01이 반복되면 11으로 치환
        call    LOOP # 연속된 1의 개수를 세는 subroutine LOOP call
        bge     r2, r6, ALT_END # 01이 반복되는 bit 수가 10이 반복되는 bit수 보다 많으면, go to ALT_END
        mov     r2, r6 # 10이 반복되는 수가 더 많으면, 리턴 값에 r6 저장
ALT_END:mov     ra, r14 # return address에 원래 돌아가야할 주소 저장
        ret


LOOP:   beq     r8, r0, LOOP_END /* r8 = 0이면, subroutine 종료 */
        srli    r9, r8, 0x01 /* r9 <= r8 >> 1 */
        and     r8, r8, r9 /* r8 <= r8 & r9 하여 1이 연속되어 나오는 부분만 살림 */
        addi    r2, r2, 0x01 /* r2 = r2 + 1 하여 연속된 1의 개수 count */
        br      LOOP /* LOOP로 가서 반복 */
LOOP_END:ret      /* return */

_start: ldw r4, TEST_NUM(r0) # r4 <= TEST_NUM(0)
        mov r10, r0 
        mov r11, r0
        mov r12, r0 # r10, r11, r12 초기화
        call ZEROS 
        mov r10, r2 /*r10 = count of the longest 0’s*/
        call ONES
        mov r11, r2 /*r11 = count of the longest 0’s*/
        call ALTERNATE
        mov r12, r2 /*r12 = count of the longest 10’s*/
END:    br END
TEST_NUM:.word 0x55555555 # 0b1111|1111|0101|0101|0101|1010|0000|0000
        .end              # 연속된 1의 bit수 : 8, 10이 반복되는 bit 수: 13, # 0이 반복되는 bit 수: 9