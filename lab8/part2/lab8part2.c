#include "address_map_nios2.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;   
volatile int pixel_buffer_start;  
short int front_pixel_buffer[512 * 256];  
short int back_pixel_buffer[512 * 256];  

void draw_pixel(int x, int y, short int color){
    *(short int *)(pixel_buffer_start + (((y)&0xFF)<<10) + (((x)&0x1FF)<<1)) =  color;
}

void draw_square(int x1, int y1, int x2, int y2, short int color){
    int x, y;
    for(x = x1; x <= x2; x++)
        for(y = y1; y <= y2; y++)
            draw_pixel(x, y, color);
        
}

void clear_screen(short int color){
    draw_square(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, color);
}

void swap(int *x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void draw_line(x0, y0, x1, y1, color){

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

void wait_for_vsync(){
    register int status;
    *pixel_ctrl_ptr = 1;

    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0){
        status = *(pixel_ctrl_ptr + 3);
    }
}

int main(void){


    volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;
    *(pixel_ctrl_ptr + 1) = front_pixel_buffer;
    wait_for_vsync();
    
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(0);
    *(pixel_ctrl_ptr + 1) = back_pixel_buffer;

    int y = 0, dy = 1;
    while(1){
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        clear_screen(0);
        draw_line(100, y, 220, y, 0xFFFF);
        if(y == SCREEN_HEIGHT - 1) 
        dy = -1;
        else if(y == 0)
        dy = 1;
        y = y + dy;
        wait_for_vsync();
    }
}