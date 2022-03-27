.include "address_map_nios2.s"
.text
.global _start
_start: 
         movia r7, KEY_BASE # r7<-Key Memory Address
         movia r8, HEX3_HEX0_BASE # r8<-7SEGMENT Memory Address
         movia   r9, TIMER_BASE # r9<-Timer Memory Address
         movhi sp, 0x2 # sp<-0x20000
         movi r12, 100 # r12<-100
 
         movui    r10, 0xE0FF          # Timer의 Conter start value를 1초로 설정
         sthio   r10, 8(r9)  # ...
         movi    r10, 0x05F5          # ... 
         sthio   r10, 12(r9)            # ...
         movi    r10, 0b0010            # 타이머의 cont를 1로 설정
         stw r10, 4(r9)      # ...
         movi r11, 0          # 표시할 숫자의 저장할 r11를 0으로 초기화한다.
         mov  r4, r11
         call DISPLAY
 
/*MAIN LOOP*/
MAIN:
         ldwio r10, 0(r9)          # status register 확인
         andi r10, r10, 0b10 # RUN을 확인
         beq r10, r0, KEY_PRESS_WAIT # RUN이 0이면 TIMER가 멈춰있는 상태이므로 키입력을 기다린다.
         ldwio r10, 0(r9)          # 아니면 TO 확인
         andi r10, r10, 0x1 # ...
         beq r10, r0, KEY_PRESS_WAIT # TO가 1이면 TIME OUT된것, 0이면 카운트를 건너뛰고 키입력을 확인한다.
         addi r11, r11, 1 # 1 카운트
         bne r11, r12, MAIN_CONT # 100이 되면
         movi r11, 0 # r11을 0으로 초기화
 
MAIN_CONT: mov  r4, r11 # r11을 DISPLAY
         call DISPLAY
     stbio    r0, 0(r9)  # DISPlAY 후 TO를 0으로 초기화
 
KEY_PRESS_WAIT:
         ldwio  r6, 0(r7)          #Key pressed확인
         beq  r6, r0, MAIN #...
 
KEY_RELEASE_WAIT:
         ldwio  r5, 0(r7)           #Key released확인
         bne  r5, r0, KEY_RELEASE_WAIT #...
 
         ldwio r10, 0(r9)          #status RUN 확인
         andi r10, r10, 0b10 #...
 
         beq r10, r0, MAIN_CONT2 #RUN 상태에 따라 타이머를 STOP, START
          movi    r10, 0b1010                #타이머 STOP=1, CONT=1
         stw    r10, 4(r9)     
         br MAIN
 
MAIN_CONT2: movi    r10, 0b0110            #타이머 START=1, CONT=1
         stw    r10, 4(r9)    
         br MAIN /*MAIN LOOP END*/
 
/*DISPLAY SUBROUTINE*/
DISPLAY:
         subi  sp, sp, 4
         stw ra, 0(sp)
         call  DIVIDE
         mov  r4, r2
         call  SEG7_CODE
         mov  r14, r2
         mov  r4, r3
         call  SEG7_CODE
         slli  r2, r2, 8
         or  r14, r14, r2
         stw  r14, 0(r8)
         ldw ra, 0(sp)
         addi  sp, sp, 4
         ret
 
/*DVIDE SUBROUTINE*/
DIVIDE:           
         mov r2, r4
         movi r5, 10
         movi r3, 0
DIVIDE_CONT:  blt r2, r5, DIV_END 
         sub r2, r2, r5
         addi r3, r3, 1
         br DIVIDE_CONT
DIV_END:         ret
 
/*SEG7_CODE SUBROUTINE*/
SEG7_CODE:  movia r15, BIT_CODES 
         add r15, r15, r4 
         ldb r2, (r15) 
         ret
 
BIT_CODES:  .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
.byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
.skip 2
.end