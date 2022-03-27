
            .text 
            .global _start
_start:     
            movia       r4, N /* r4 <= address[N] */
            addi        r8, r4, 4 /* r8 <= r4 + 4 */
            ldw         r4, (r4) /* r4 <= MEM[r4] = 9876 */

            call        DIVIDE 
            mov         r4, r3 /* r4 <= 987 = 9876 / 10 (quotient) */
            stb         r2, (r8) /* Digits[0] <= 6 = 9876 % 10 (remainder) */
            call        DIVIDE
            mov         r4, r3 /* r4 <= 98 = 987 / 10 (quotient) */
            stb         r2, 1(r8) /* Digits[1] <= 7 = 987 % 10 (remainder) */
            call        DIVIDE
            stb         r2, 2(r8) /* Digits[2] <= 8 = 98 % 10 (remainder) */
            stb         r3, 3(r8) /* Digits[3] <= 9 = 98 / 10 (remainder) */

END:        br          END

DIVIDE:     mov         r2, r4 /* subroutine DIVIDE, r2 <= r4 (r2 = remainder, r4 = dividend) */
            movi        r5, 10 /* r5 <= 10 (r5 = divisor) */
            movi        r3, 0 /* r3 <= 0 (r3 = quotient) */
CONT:       blt         r2, r5, DIV_END /* if r2 < r5, go to DIV_END */
            sub         r2, r2, r5 /* r2 <= r2 - 10 */
            addi        r3, r3, 1 /* r3 <= r3 + 1 */
            br          CONT /* go to CONT */ 
DIV_END:    ret /* if r2 < r5, return. quotients is in r3, remainder is in r2 */

N:          .word       9876 /* the decimal number to be converted */
Digits:     .space      4 /* storage space for the decimal digits */

            .end
