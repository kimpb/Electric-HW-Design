
        .text
        .global _start

_start: 
        mov r2, r0
        movia r3, N
        ldw r4, 0(r3)
        call FINDSUM
END:    br END

FINDSUM:
        add r2, r2, r4
        subi r4, r4, 1
        bne r4, r0, FINDSUM
        ret

N:      .word 9
        .end