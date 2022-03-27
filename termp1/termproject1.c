
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAIN_X 12 //게임판 가로크기 
#define MAIN_Y 24 //게임판 세로크기 
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 1 //게임판 위치조정
#define WHITE 0xFFFF
#define BLUE 0x001F
#define RED 0xF800
#define PINK 0xF81F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define BLACK 0x0
#define ORANGE 0xFBE0
#define GRAY 0x7BEF
#define DARKGRAY 0x39E7

#define X_OFFSET 96
#define Y_OFFSET 24
#define XOFFSET 40
#define YOFFSET 64

#define true 1
#define false 0

#define LEFT 0x6B
#define RIGHT 0x74
#define DOWN 0x72
#define UP 0x75
#define SPACE 0x29
#define ESC 0x76
#define ENTER 0x5A
#define F1 0x05



 


volatile unsigned int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // PixelBufferBaseAddress 
volatile unsigned int pixel_buffer_start;  // 만들어지는 화면이 저장되는 주소의 global variable
volatile unsigned int * PS2_ptr = (int *) PS2_BASE;
volatile unsigned int *KEY_ptr = (int *) KEY_BASE;
volatile unsigned int * TIMER_ptr = (int *) TIMER_BASE;
volatile unsigned int * TIMER2_ptr = (int *) TIMER_2_BASE;
short int front_pixel_buffer[512 * 256];  // allocate memory for front buffer
short int back_pixel_buffer[512 * 256];  // allocate memory for back buffer

int position[10][22];
int dx, dy;

int blocktype;
int blocktype_next;
int rotation;
int movingblock;
int level=0;
char level1[4];
int crush_on;
int blockcolor;
int new_block_on;
unsigned int count;
unsigned int cntvalue1;
unsigned int cntvalue2;
int levelcount=0;
int score;
char score1[8];

int dyte3, dyte2, dyte1;

int board[MAIN_Y][MAIN_X]; // 테트리스 게임 판
int blocks[7][4][4][4]={ // blocks[blocktype][rotation][Y][X]
{{{0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,1,1,0},
  {0,1,1,0},
  {0,0,0,0}}},
{{{0,0,0,0},
  {0,0,0,0},
  {1,1,1,1},
  {0,0,0,0}},
 {{0,1,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0}},
 {{0,0,0,0},
  {0,0,0,0},
  {1,1,1,1},
  {0,0,0,0}},
 {{0,1,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0}}},
{{{0,0,0,0},
  {1,1,0,0},
  {0,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,0,1,0},
  {0,1,1,0},
  {0,1,0,0}},
 {{0,0,0,0},
  {1,1,0,0},
  {0,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,0,1,0},
  {0,1,1,0},
  {0,1,0,0}}},
{{{0,0,0,0},
  {0,1,1,0},
  {1,1,0,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0}},
 {{0,0,0,0},
  {0,1,1,0},
  {1,1,0,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0}}},
{{{0,0,0,0},
  {0,0,1,0},
  {1,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,0,0}},
  {{0,0,0,0},
  {0,0,0,0},
  {1,1,1,0},
  {1,0,0,0}},
 {{0,0,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {0,1,1,0}}},
{{{0,0,0,0},
  {1,0,0,0},
  {1,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {1,1,0,0}},
 {{0,0,0,0},
  {0,0,0,0},
  {1,1,1,0},
  {0,0,1,0}},
 {{0,0,0,0},
  {0,1,1,0},
  {0,1,0,0},
  {0,1,0,0}}},
{{{0,0,0,0},
  {0,1,0,0},
  {1,1,1,0},
  {0,0,0,0}},
 {{0,0,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,1,0,0}},
  {{0,0,0,0},
  {0,0,0,0},
  {1,1,1,0},
  {0,1,0,0}},
 {{0,0,0,0},
  {0,1,0,0},
  {1,1,0,0},
  {0,1,0,0}}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][rotation][i][j]로 사용
void clear_screen(short int color); // VGA monitor 화면 초기화 함수
void video_text(int x, int y, char *text_ptr); // VGA monitor에 text 출력 함수
void draw_square(int x1, int y1, int x2, int y2, short int color); // VGA monitor에 사각형 그리는 함수
void wait_for_vsync(); // V-Sync를 위해 기다리는 함수
void draw_board(); // 테트리스 판 그리는 함수 
int check_crush(int dx, int dy, int rotation); //지정된 좌표와 회전값으로 충돌이 있는지 검사하는 함수 
void check_line(); // 줄이 가득 찼는지 판단하고 지우는 함수
void check_game_over(); // game over인지 check
void move_block(int dir); // block을 방향에 따라 움직히는 함수
void drop_block(); // SPACE BAR 눌렀을 때
void createblock(); // 게임 판의 상단 중앙에 block 생성
void nextblock(); // 다음 block 표시 함수
void reset_board(); //게임판을 초기화  
void reset(); // reset 함수
void swap(int *x, int* y){ // draw_line 함수를 위한 swap 함수
    int temp = *x;
    *x = *y;
    *y = temp;
}
void draw_pixel(int x, int y, short int color){ // VGA monitor에 pixel을 그리는 함수
    *(short int *)(pixel_buffer_start + (y<<10) + (x<<1)) =  color;
}
void draw_line(int x0, int y0, int x1, int y1, short int color){ // 선을 그리는 함수

    int is_steep;
    is_steep = (abs(y1-y0) > abs(x1-x0));
    if (is_steep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0> x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int error = -(deltax /2);
    int y_step;
    int x;
    int y = y0;
    if(y0 < y1)
    y_step = 1;
    else y_step = -1;

    for(x = x0; x <= x1; x++){
        if(is_steep)
        draw_pixel(y, x, color);
        else
        draw_pixel(x, y, color);
        error = error + deltay;
        if(error >= 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}
void draw_square(int x1, int y1, int x2, int y2, short int color){ // 사각형 그리는 함수

    int x, y;
    for(x = x1; x <= x2; x++)
        for(y = y1; y <= y2; y++)
            draw_pixel(x, y, color);
        
}


void wait_for_vsync(){
    register int status;
    *pixel_ctrl_ptr = 1;

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0){
        status = *(pixel_ctrl_ptr + 3);
    }
}



void drop_block(void){ // SPACE BAR 눌렀을 때
    int i=0, j=0;
    while(check_crush(dx, dy+j, rotation)){
        j++;
    }
    for(i=0;i<j-1;i++){
        move_block(DOWN);
    }
            
            movingblock = 0;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(board[dy+i][dx+j])
                    board[dy+i][dx+j] += 10;
                }
            }

}
 

int check_crush(int dx, int dy, int rotation){ //지정된 좌표와 회전값으로 충돌이 있는지 검사하는 함수 
    int i,j;
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
            if((blocks[blocktype][rotation][i][j]==1)&&(board[dy+i][dx+j]>10)) 
            return false;
        }
    }    
    return true; //하나도 안겹치면 true리턴 
};

 
void move_block(int dir){ //블록을 이동시키는 함수
    int i,j;
 
    switch(dir){
        case LEFT: //왼쪽방향 
            for(i=0;i<4;i++){ //현재좌표의 블럭을 지움 
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){ //왼쪽으로 한칸가서 active block을 찍음 
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j-1]=blocktype+1;
                }
            }
            dx--; //좌표값 이동 
            break;
        
        case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j+1]=blocktype+1;
                }
            }
            dx++;        
            break;
        
        case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i+1][dx+j]=blocktype+1;
                }
            }
            dy++;        
            break;
        
        case UP: //키보드 위쪽 눌렀을때 회전시킴. 
            for(i=0;i<4;i++){ //현재좌표의 블럭을 지움  
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=EMPTY;
                }
            }
            rotation=(rotation+1)%4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 
            for(i=0;i<4;i++){ //회전된 블록을 찍음 
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=blocktype+1;
                }
            }
            break;
            
        case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
                  //이를 동작시키는 특수동작 
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i][dx+j]=EMPTY;
                }
            }
            rotation=(rotation+1)%4;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[blocktype][rotation][i][j]==1) board[dy+i-1][dx+j]=blocktype + 1;
                }
            }
            dy--;
            break;
    }
}



void reset_board(void){ //게임판을 초기화  
    int i,j;
    
    for(i=0;i<MAIN_Y;i++){ // 게임판을 0으로 초기화  
        for(j=0;j<MAIN_X;j++){
            if( (j == 0) || (j == (MAIN_X - 1)) || (i == (MAIN_Y - 1)) )
            board[i][j] = 18;
            else
            board[i][j]=0;


        }
    }    
    board[0][0] = 0;
    board[0][11] = 0;
}
 

void draw_board(void){ //게임판 그리는 함수 
    int i, j;    
    
    for(i=0;i<MAIN_Y;i++){
        for(j=0;j<MAIN_X;j++){

            switch(board[i][j]%10){

            case 0:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, BLACK);
                break;
            case 1:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, YELLOW);
                break;
            case 2:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, CYAN);
                break;
            case 3:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, RED);
                break;
            case 4:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, GREEN);
                break;
            case 5:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, ORANGE);
                break;
            case 6:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, BLUE);
                break;
            case 7:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, PINK);
                break;
            case 8:
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, GRAY);
                break;
            default :
                draw_square(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, WHITE);
            }
            if(board[i][j]%10){
            draw_line(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8, DARKGRAY);
            draw_line(X_OFFSET+j*8+7, Y_OFFSET+i*8, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, DARKGRAY);
            draw_line(X_OFFSET+j*8, Y_OFFSET+i*8+7, X_OFFSET+j*8+7, Y_OFFSET+i*8+7, DARKGRAY);
            draw_line(X_OFFSET+j*8, Y_OFFSET+i*8, X_OFFSET+j*8, Y_OFFSET+i*8+7, DARKGRAY);
            }
        }
    }
}

void video_text(int x, int y, char *text_ptr){
    int offset;
    volatile char *character_buffer = (char *) FPGA_CHAR_BASE;

    offset = (y<<7) + x;
    while(*(text_ptr)){
        *(character_buffer + offset) = *(text_ptr);
        ++text_ptr;
        ++offset;
    }

}

void check_line(void){ 
    int i, j, k, l;
    
    int    block_amount; //한줄의 블록갯수를 저장하는 변수 
    
    for(i=MAIN_Y-2;i>3;){ //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사 
        block_amount=0; //블록갯수 저장 변수 초기화 
        for(j=1;j<MAIN_X-1;j++){ //벽과 벽사이의 블록갯루를 셈 
            if(board[i][j]>0) block_amount++;
        }
        if(block_amount==MAIN_X-2){ //블록이 가득 찬 경우 
            for(k=i;k>4;k--){ //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
                for(l=1;l<MAIN_X-1;l++){
                   board[k][l]=board[k-1][l];

                }
            }
            
                    levelcount++;
                    score = score + (level+1) * 1000;
        sprintf(score1, "%d", score);
        video_text(14, 35, score1);
        }
        if(levelcount/3){
            video_text(5, 30, "LEVEL :");
            level++;
            char level1[6];
            sprintf(level1, "%d", level);
            video_text(14, 30, level1);
            levelcount = 0;
            
            if(cntvalue1>10000000){
            cntvalue1 = cntvalue1 - 5000000;
        *(TIMER_ptr + 1) = 0b1011;
        *(TIMER_ptr + 2) = cntvalue1 & 0xFFFF;
        *(TIMER_ptr + 3) = (cntvalue1>>16) & 0xFFFF;
        *(TIMER_ptr + 1) = 0b0111;
            }
        }
        else i--;
    }
}
 

void clear_screen(short int color){
    draw_square(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, color);
}

void createblock(){ // 게임 판의 상단 중앙에 block 생성
    int i, j;
    dx = 4;
    dy = 0;
    rotation = 0;
    movingblock = 1;
    blocktype = blocktype_next;
    blocktype_next = rand()%7;
    blockcolor = blocktype + 1;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            if(board[i][dx+j]==0)
            board[i][dx+j] = blocks[blocktype][rotation][i][j] * (blocktype + 1);

}

void check_game_over(void){ // game over인지 check
    int i;
    for(i=1;i<MAIN_X-2;i++){
        if(board[3][i]>10){
          *(TIMER_ptr + 1) = 0b1011; 
          *(TIMER2_ptr + 1) = 0b1011; 
          video_text(60, 30, "GAME OVER\0");
        }
    }
}

void reset(){ // reset 함수
    
    *(pixel_ctrl_ptr + 1) = front_pixel_buffer;
    wait_for_vsync();


    
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0);
    *(pixel_ctrl_ptr + 1) = back_pixel_buffer;

    reset_board();
    blocktype_next = rand()%7;
    *(KEY_ptr + 2) = 0b1001;

    cntvalue1 = 50000000;
    *(TIMER_ptr + 1) = 0b1011;
    *(TIMER_ptr + 2) = cntvalue1 & 0xFFFF;
    *(TIMER_ptr + 3) = (cntvalue1>>16) & 0xFFFF;

    *(PS2_ptr) = 0xFF; // reset
    *(PS2_ptr + 1) = 1;

        video_text(5, 30, "LEVEL :");
    level = 0;
        sprintf(level1, "%d", level);
        video_text(14, 30, level1);
        video_text(15, 30, "       \0");
        levelcount = 0;

        video_text(5, 35, "SCORE :");
        score = 0;
        sprintf(score1, "%d", score);
        video_text(14, 35, score1);
        video_text(15, 35, "       \0");




    NIOS2_WRITE_IENABLE(0b10000111);
    NIOS2_WRITE_STATUS(1);
    movingblock = 0;
    video_text(60, 15, "TETRIS");
    video_text(55, 30, "PRESS BUTTON TO START\0");
    video_text(55, 19, "UP : ROTATE\0");
    video_text(55, 20, "<- : RIGHT / -> : LEFT\0");
    video_text(55, 21, "DOWN : SOFT DROP\0");
    video_text(55, 22, "SPACE : HARD DROP\0");
    video_text(60, 35, "                  \0");
    
}

void nextblock(){ // 다음 block 표시 함수
    video_text(9, 14, "NEXT BLOCK");
        int i, j;    
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){

            switch(blocks[blocktype_next][0][i][j]*(blocktype_next+1)){

            case 0:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, BLACK);
                break;
            case 1:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, YELLOW);
                break;
            case 2:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, CYAN);
                break;
            case 3:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, RED);
                break;
            case 4:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, GREEN);
                break;
            case 5:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, ORANGE);
                break;
            case 6:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, BLUE);
                break;
            case 7:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, PINK);
                break;
            case 8:
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, GRAY);
                break;
            default :
                draw_square(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, WHITE);
            }
            if(blocks[blocktype_next][0][i][j]){
            draw_line(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8, DARKGRAY);
            draw_line(XOFFSET+j*8+7, YOFFSET+i*8, XOFFSET+j*8+7, YOFFSET+i*8+7, DARKGRAY);
            draw_line(XOFFSET+j*8, YOFFSET+i*8+7, XOFFSET+j*8+7, YOFFSET+i*8+7, DARKGRAY);
            draw_line(XOFFSET+j*8, YOFFSET+i*8, XOFFSET+j*8, YOFFSET+i*8+7, DARKGRAY);
        }
        }
    }
}


void main(){
    int i, j;

    reset();

    while(1){
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        clear_screen(0);
        nextblock();
        draw_board();
        if(movingblock == 0)
        createblock();
        check_line();
        check_game_over();
        wait_for_vsync();
    }
}