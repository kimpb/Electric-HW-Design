
#include "address_map_nios2.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int pixel_buffer_start;
volatile int *pixel_ctrl_ptr;


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


int main(void){

    volatile int *pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen(0);
    draw_line(0, 0, 150, 150, 0x001F);
    draw_line(150, 150, 319, 0, 0x07E0);
    draw_line(0, 239, 319, 239, 0xF800);
    draw_line(319, 0, 0, 239, 0xF81F);
    
}
