#include "address_map_nios2.h"

#define MAIN_X 12 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
#define true 1
#define false 0
#define DOWN 0x72

extern volatile int *TIMER_ptr;
extern int board[23][12];
extern int blocks[7][4][4][4];
extern int movingblock;
extern int dx, dy;
extern int blocktype;
extern int rotation;
int movingblockcount;



void TIMER_ISR(void){
    *TIMER_ptr = 0;
    int i, j;
    int crush;
    
    if(*TIMER_ptr & 0b10){
        crush = check_crush(dx, dy+1, rotation);
        if(crush){
            movingblock = 1;
            move_block(DOWN);
        }
        else{
            movingblock = 0;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(board[dy+i][dx+j])
                    board[dy+i][dx+j] += 10;
                }

            }
        }
    }
    return;
}