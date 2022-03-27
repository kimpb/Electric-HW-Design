
#include "address_map_nios2.h"
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAIN_X 12 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
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

#define X_OFFSET 112
#define Y_OFFSET 24

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 
 


volatile int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // PixelBufferBaseAddress 
volatile int pixel_buffer_start;  // 만들어지는 화면이 저장되는 주소의 global variable
volatile int * ps2_ptr = (int *) PS2_BASE;
volatile int * TIMER_ptr = (int *) TIMER_BASE;
volatile int * TIMER2_ptr = (int *) TIMER_2_BASE;
short int front_pixel_buffer[512 * 256];  // allocate memory for front buffer
short int back_pixel_buffer[512 * 256];  // allocate memory for back buffer

int position[22][10];

void clear_screen(short int color);
void draw_square(int x1, int y1, int x2, int y2, short int color);
void wait_for_vsync();
void swap(int *x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
void draw_pixel(int x, int y, short int color){
    *(short int *)(pixel_buffer_start + (y<<10) + (x<<1)) =  color;
}

void draw_line(int x0, int y0, int x1, int y1, short int color){

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

void clear_screen(short int color){
    draw_square(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, color);
}

void draw_square(int x1, int y1, int x2, int y2, short int color){
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

void gotoxy(int x,int y) { //gotoxy함수 
    COORD pos={2*x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

int blocks[7][4][4][4]={
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 


int main_org[23][12];
int main_cpy[23][12];

void reset_main(void){ //게임판을 초기화  
    int i,j;
    
    for(i=0;i<MAIN_Y;i++){ // 게임판을 0으로 초기화  
        for(j=0;j<MAIN_X;j++){
            main_org[i][j]=0;
        }
    }    
    for(i=1;i<MAIN_Y-1;i++){ //좌우 벽을 만듦  
        main_org[i][0]=WALL;
        main_org[i][MAIN_X-1]=WALL;
    }
    for(j=0;j<MAIN_X;j++){ //바닥벽을 만듦 
        main_org[MAIN_Y-1][j]=WALL;
    }
}
 

void draw_main(void){ //게임판 그리는 함수 
    int i, j;    
    
    for(i=0;i<MAIN_X;i++){
        for(j=0;j<MAIN_Y;j++){

            switch(main_org[i][j]){

            case 0:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, 0);
                break;
            case 1:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, GRAY);
                break;
            case 2:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, RED);
                break;
            case 3:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, GREEN);
                break;
            case 4:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, BLUE);
                break;
            case 5:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, PINK);
                break;
            case 6:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, YELLOW);
                break;
            case 7:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, CYAN);
                break;
            case 8:
                draw_square(X_OFFSET+i*8, Y_OFFSET+j, X_OFFSET+i*8-7, Y_OFFSET+j-7, ORANGE);
                break;
                
            }
        }
    }
    

}


int main(){

    clear_screen(0);

    reset_main();

    draw_main();
}