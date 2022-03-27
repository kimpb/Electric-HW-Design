#include "address_map_nios2.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // PixelBufferBaseAddress 
volatile int pixel_buffer_start;  // 만들어지는 화면이 저장되는 주소의 global variable
short int front_pixel_buffer[512 * 256];  // allocate memory for front buffer
short int back_pixel_buffer[512 * 256];  // allocate memory for back buffer
 


void clear_screen(short int color);
void draw_square(int x1, int y1, int x2, int y2, short int color);
void wait_for_vsync();

short int front_pixel_buffer[512*256];
short int back_pixel_buffer[512*256];

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

int main(void){
    int x[8];
    int y[8];
    int dx[8];
    int dy[8];
    int i;

    for(i=0; i<8; i++){
        x[i] = rand()%SCREEN_WIDTH;
        y[i] = rand()%SCREEN_HEIGHT;
        dx[i] = rand()%5;
        dy[i] = rand()%5;
    }

    *(pixel_ctrl_ptr + 1) = front_pixel_buffer;
    wait_for_vsync();
    
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0);
    *(pixel_ctrl_ptr + 1) = back_pixel_buffer;

    while(1){
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        clear_screen(0);
        for(i = 0; i < 8; i++){
            draw_square(x[i]-2, y[i]-2, x[i]+2, y[i]+2, 0x001F);
        }
        for(i = 0; i < 8; i++){
            if(i==7)
            draw_line(x[7], y[7], x[0], y[0], 0x001F);
            else
            draw_line(x[i], y[i], x[i+1], y[i+1], 0x001F);
        }

        for(i=0;i<8;i++){
            if(x[i]>=SCREEN_WIDTH-1) dx[i] = -(rand()%5);
            else if(x[i] <= 0) dx[i] = rand()%5;
            if(y[i]>=SCREEN_HEIGHT-1) dy[i] = -(rand()%5);
            else if(y[i] <= 0) dy[i] = rand()%5;
            x[i] = (x[i] + dx[i]);
            y[i] = (y[i] + dy[i]);

        }
        wait_for_vsync();
    }
}