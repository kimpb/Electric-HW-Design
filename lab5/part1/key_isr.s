/*-----------------------------------------
                 key_isr.s
------------------------------------------*/

        .include "address_map_nios2.s"
        .global ON_KEY_PRESS # global subroutine 선언
/* key3-0을 누르면 interrupt가 발생하여 call 되는 subroutine ON_KEY_PRESS */
ON_KEY_PRESS:
                subi sp, sp, 12
                stw r22, 0(sp)
                stw r23, 4(sp)
                stw ra, 8(sp) # r22, r23, return address stack에 저장
                

                movia r22, KEY_BASE # r22에 KEY3-0의 base address 저장
                ldwio r23, 0xC(r22)
                stwio r23, 0xC(r22) # r23에 pushbutton edge capture register값 저장 후 초기화
                ldwio r17, 0(r21) # r21 = HEX3-0 base, 현재 HEX에 출력되고 있는 값 r17에 저장

                andi r16, r23, 0b0001 # r16 <= pushbutton의 edgecapture register & 0b0001 
                beq r16, r0, SKIP0 # r16이 0이면,(누른게 key0가 아니므로) SKIP
                movi r4, 0 # 누른게 key0이 맞다면, 7segment 변환 subroutine SEG7_CODE를 위한 r4 <= 0
                call SEG7_CODE # r4를 넘겨받아 r4에 대응하는 7segment값을 r2로 return 
                and r18, r17, r10 # r10 = 0x000000FF과, r17 = 현재 HEX에 출력되고 있는 값을 AND
                beq r18, r0, ON0 # AND하여 나온 값이 0이면, HEX0는 꺼져있으므로, 켜주는 branch ON0로 이동
                xor r2, r2, r17 # HEX0가 켜져있으면, r2와 r17을 xor하여 r2에 저장하여 HEX0만 꺼줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
ON0:            or r2, r2, r17 # HEX0가 꺼져있으면, r2와 r17을 or하여 HEX0만 켜줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END #종료

SKIP0:          andi r16, r23, 0b0010 # r16 <= pushbutton의 edgecapture register & 0b0010
                beq r16, r0, SKIP1 # r16이 0이면,(누른게 key1이 아니므로) SKIP
                movi r4, 1 # 누른게 key1이 맞다면, 7segment 변환 subroutine SEG7_CODE를 위한 r4 <= 1
                call SEG7_CODE # r4를 넘겨받아 r4에 대응하는 7segment값을 r2로 return 
                slli r2, r2, 8 # r2를 왼쪽으로 8칸 shift
                and r18, r17, r11 # r11 = 0x0000FF00과, r17 = 현재 HEX에 출력되고 있는 값을 AND
                beq r18, r0, ON1 # AND하여 나온 값이 0이면, HEX1는 꺼져있으므로, 켜주는 branch ON1로 이동
                xor r2, r2, r17 # HEX1가 켜져있으면, r2와 r17을 xor하여 r2에 저장하여 HEX1만 꺼줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
ON1:            
                or r2, r2, r17 # HEX1가 꺼져있으면, r2와 r17을 or하여 HEX1만 켜줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
            
SKIP1:          andi r16, r23, 0b0100 # r16 <= pushbutton의 edgecapture register & 0b0100
                beq r16, r0, SKIP2 # r16이 0이면,(누른게 key2가 아니므로) SKIP
                movi r4, 2 # 누른게 key2가 맞다면, 7segment 변환 subroutine SEG7_CODE를 위한 r4 <= 2
                call SEG7_CODE # r4를 넘겨받아 r4에 대응하는 7segment값을 r2로 return 
                slli r2, r2, 16 # r2를 왼쪽으로 16칸 shift
                and r18, r17, r12 # r12 = 0x00FF0000과, r17 = 현재 HEX에 출력되고 있는 값을 AND
                beq r18, r0, ON2 # AND하여 나온 값이 0이면, HEX2는 꺼져있으므로, 켜주는 branch ON2로 이동
                xor r2, r2, r17 # HEX2가 켜져있으면, r2와 r17을 xor하여 r2에 저장하여 HEX2만 꺼줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
ON2:            
                or r2, r2, r17 # HEX2가 꺼져있으면, r2와 r17을 or하여 HEX2만 켜줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료

SKIP2:          andi r16, r23, 0b1000 # r16 <= pushbutton의 edgecapture register & 0b1000
                beq r16, r0, SKIP3 # r16이 0이면,(누른게 key3가 아니므로) SKIP
                movi r4, 3 # 누른게 key3가 맞다면, 7segment 변환 subroutine SEG7_CODE를 위한 r4 <= 3
                call SEG7_CODE #r4를 넘겨받아 r4에 대응하는 7segment값을 r2로 return 
                slli r2, r2, 24 # r2를 왼쪽으로 24칸 shift
                and r18, r17, r13 # r13 = 0xFF000000과, r17 = 현재 HEX에 출력되고 있는 값을 AND
                beq r18, r0, ON3 # AND하여 나온 값이 0이면, HEX3는 꺼져있으므로, 켜주는 branch ON3로 이동
                xor r2, r2, r17 # HEX3가 켜져있으면, r2와 r17을 xor하여 r2에 저장하여 HEX3만 꺼줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
ON3:            
                or r2, r2, r17 # HEX3가 꺼져있으면, r2와 r17을 or하여 HEX3만 켜줌
                stwio r2, 0(r21) # r2를 HEX3-0을 통해 출력
                br KEY_PRESS_END # 종료
SKIP3:          stwio r0, 0(r21) # 다 아니면, HEX를 다 끄고 종료


KEY_PRESS_END:  ldw r22, 0(sp)
                ldw r23, 4(sp)
                ldw ra, 8(sp) 
                addi sp, sp, 12 # r22, r23, ra, stack pointer 복구하여 return
                ret

            






/* r4를 넘겨받아 r4에 해당하는 7segment값을 r2로 넘겨주는 subroutine SEG7_CODE*/
SEG7_CODE: 
                movia   r15, BIT_CODES # starting address of bit codes
                add     r15, r15, r4 # index into the bit codes
                ldb     r2, (r15) # read the bit code needed for our digit
        ret

BIT_CODES:      .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
                .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
                .skip   2 # pad with 2 bytes to maintain word alignment

        .end
