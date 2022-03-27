    .text
    .global _start

_start:
            movia r8, RESULT @r8 = ADDRESS[RESULT] 
            ldw r4, 4(r8) 
            addi r5, r8, 8  
            ldw r2, (r5) 
LOOP:       subi r4, r4, 1 
            beq r4, r0, DONE 
            addi r5, r5, 4 
            ldw r6, (r5) 
            bge r2, r6, LOOP 
            mov r2, r6 
            br LOOP 
DONE:       stw r2, (r8) 

STOP:       br STOP

RESULT:     .skip 4
N:          .word 7
NUMBERS:    .word 4, 5, 3, 6
            .word 1, 8, 2

            .end