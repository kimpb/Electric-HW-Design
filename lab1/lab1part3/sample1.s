
    .text
    .global _start

_start:
            movia       r8, RESULT /* r8 <= ADDRESS[RESULT] */ 
            ldw         r4, 4(r8) /* r4 <= 7 */ 
            addi        r5, r8, 8 /* r5 <= r8 + 8 */
            call        LARGE /* call LARGE */
            stw         r2, (r8) /* MEM[r8] <= r2 */

STOP:       br          STOP

LARGE:      subi         r4, r4, 1 /* r4 <= r4 - 1 */
            beq         r4, r0, DONE /* if r4 = 0, go to DONE */
            addi        r5, r5, 4 /* else, r5 <= r5 + 4 */
            ldw         r6, (r5) /* r6 <= MEM[r5] */
            bge         r2, r6, LARGE /* if r2 ≥ r6, go to LARGE */
            mov         r2, r6 /* else, r2 <= r6 */
            br          LARGE /* go to LARGE */
DONE:       ret /* if r4 = 0, ret */



RESULT:     .skip       4 /* space for the largest number found */
N:          .word       7 /* number of entries in the list */
NUMBERS:    .word       4, 5, 3, 6 /* the data */
            .word       1, 8, 2

            .end
