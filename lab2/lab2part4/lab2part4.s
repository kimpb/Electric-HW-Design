        .include "address_map_nios2.s"
        .text
        .global _start

/* 8bit SEG7_CODE로 변환하는 subroutine */
SEG7_CODE:movia r15, BIT_CODES # starting address of bit codes
        add     r15, r15, r4 # index into the bit codes
        ldb     r2, (r15) # read the bit code needed for our digit
        ret

/* 차례대로 0 1 2 3 4 5 6 7 8 9의 7-segment bit */
BIT_CODES:.byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
        .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
        .skip 2 # pad with 2 bytes to maintain word alignment


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
        mov     r2, r0 # 리턴값 초기화
        movhi   r7, 0x5555 # r7 <= 0x55550000
        ori     r7, r7, 0x5555 # r7 <= 0x55555555 (01이 16번 반복되는 32bit 수)
        xor     r8, r4, r7 # 넘겨받은 r4와 r7을 xor하여 10이 반복되면 11으로 치환
        call    LOOP # 연속된 1의 개수를 세는 subroutine LOOP call
        mov     r6, r2 # 10이 반복되는 개수를 r6에 저장
        mov     r2, r0 # 리턴값 초기화
        srli    r7, r7, 0x01 # 01이 반복되는 열을 찾기 위해 r7을 오른쪽으로 한칸 밀어 0010...1010 으로 치환
        xor     r8, r4, r7 # 넘겨받은 r4와 r7을 xor하여 01이 반복되면 11으로 치환
        call    LOOP # 연속된 1의 개수를 세는 subroutine LOOP call
        bge     r2, r6, ALT_END # 01이 반복되는 bit 수가 10이 반복되는 bit수 보다 많으면, go to ALT_END
        mov     r2, r6 # 10이 반복되는 수가 더 많으면, 리턴값에 r6 저장
ALT_END:mov     ra, r14 # return address에 원래 돌아가야할 주소 저장
        ret


LOOP:   beq     r8, r0, LOOP_END /* r4 = 0이면, subroutine 종료 */
        srli    r9, r8, 0x01 /* r5 <= r4 >> 1 */
        and     r8, r8, r9 /* r4 <= r4 & r5 하여 1이 연속되어 나오는 부분만 살림 */
        addi    r2, r2, 0x01 /* r2 = r10 + 1 하여 연속된 1의 개수 count */
        br      LOOP /* LOOP로 가서 반복 */
LOOP_END:ret      /* return */

DIVIDE: mov     r2, r4 /* subroutine DIVIDE, r2 <= r4 (r2 = remainder, r4 = dividend) */
        movi    r5, 10 /* r5 <= 10 (r5 = divisor) */
        movi    r3, 0 /* r3 <= 0 (r3 = quotient) */
CONT:   blt     r2, r5, DIV_END /* if r2 < r5, go to DIV_END */
        sub     r2, r2, r5 /* r2 <= r2 - 10 */
        addi    r3, r3, 1 /* r3 <= r3 + 1 */
        br      CONT /* go to CONT */ 
DIV_END:ret /* if r2 < r5, return. quotients is in r3, remainder is in r2 */

        

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

DISPLAY:movia   r8, HEX3_HEX0_BASE # r8에 HEX3-HEX0의 주소 저장
        mov     r4, r10 # r4에 연속된 0의 개수 저장
        call    DIVIDE # r4 / 10
        mov     r4, r2 # r4에 10으로 나눈 나머지 저장
        call    SEG7_CODE # r4(1의 자리)를 7segment code로 변환
        mov     r14, r2 # r14에 7segment code 저장
        mov     r4, r3 # r4에 10으로 나눈 몫 저장
        call    SEG7_CODE # r4(10의 자리)를 7segment code로 변환
        slli	r2, r2, 8 # 10의 자리 7segment code를 왼쪽으로 8bit만큼 shift
        or      r14, r14, r2 # 8bit만큼 shift시킨 r2(10의 자리)와 r14(1의 자리)를 합침
        stw     r14, 0(r8) # 합친 결과를 HEX3-HEX0의 주소에 저장
        
        mov     r4, r11 # r4에 연속된 0의 개수 저장
        call    DIVIDE # r4 / 10
        mov     r4, r2 # r4에 10으로 나눈 나머지 저장
        call    SEG7_CODE # r4(1의 자리)를 7segment code로 변환
        mov     r13, r2 # r14에 7segment code 저장
        mov     r4, r3 # r4에 10으로 나눈 몫 저장
        call    SEG7_CODE # r4(10의 자리)를 7segment code로 변환
        slli	r2, r2, 8 # 10의 자리 7segment code를 왼쪽으로 8bit만큼 shift
        or      r13, r13, r2 # 8bit만큼 shift시킨 r2(10의 자리)와 r14(1의 자리)를 합침
        slli    r13, r13, 16 # 합친 결과를 16bit만큼 shift 시켜 HEX3-2에 출력되도록 함
        or      r13, r13, r14 # HEX3-HEX2, HEX1-HEX0 합침 
        stw     r13, 0(r8) # 합친 결과를 HEX3-HEX0의 주소에 저장

        movia   r8, HEX5_HEX4_BASE # r8에 HEX5-HEX4 주소 저장
        mov     r4, r12 # r4에 1과 0이 반복되어 나오는 의 개수 저장
        call    DIVIDE # r4 / 10
        mov     r4, r2 # r4에 10으로 나눈 나머지 저장
        call    SEG7_CODE # r4(1의 자리)를 7segment code로 변환
        mov     r14, r2 # r14에 7segment code 저장
        mov     r4, r3 # r4에 10으로 나눈 몫 저장
        call    SEG7_CODE # r4(10의 자리)를 7segment code로 변환
        slli    r2, r2, 8 # 10의 자리 7segment code를 왼쪽으로 8bit만큼 shift
        or      r14, r14, r2 # 8bit만큼 shift시킨 r2(10의 자리)와 r14(1의 자리)를 합침
        stw     r14, 0(r8) # 합친 결과를 HEX5-HEX4의 주소에 저장
END:    br      END


TEST_NUM:.word 0xFF555A00 # 0b1111|1111|0101|0101|0101|1010|0000|0000
        .end              # 연속된 1의 bit수 : 8, 10이 반복되는 bit 수: 13, # 0이 반복되는 bit 수: 9