
        .text
        .global _start

_start: 
        movhi sp, 0x2
        movia r3, N
        ldw r4, 0(r3)
        call FINDSUM
END:    br END

FINDSUM:bne r4, r0, RECURSE
        mov r2, r0
        ret

RECURSE:subi sp, sp, 8
        stw r4, 0(sp)
        stw ra, 4(sp)

        subi r4, r4, 1
        call FINDSUM  #dasd
 
        ldw r4, 0(sp)
        ldw ra, 4(sp)
        addi sp, sp, 8

        add r2, r4, r2
        ret

N:      .word 9
        .end
