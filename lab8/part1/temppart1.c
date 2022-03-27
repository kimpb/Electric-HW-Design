#include "address_map_nios2.h"
 
/* 전역 변수 선언 */
volatile int * pixel_ctrl_ptr = (int *) PIXEL_BUF_CTRL_BASE;  // PixelBufferBaseAddress
volatile int pixel_buffer_start;  // 만들어지는 화면이 저장되는 주소의 global variable
 
/* 상수 정의 */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
 
/*함수 정의*/
 
/*x, y픽셀에 색을 넣는 함수*/
void plot_pixel(int x, int y, short int line_color){
*(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
 
/* 화면을 흰색으로 채우는 함수 */
void clear_screen(){
int x, y;
for(x = 0; x <= SCREEN_WIDTH-1; x++){  // 화면 내부에서
for(y = 0; y <= SCREEN_HEIGHT-1; y++)  // x, y 를 이동시키면서
plot_pixel(x, y, 0xFFFF);  // 해당 pixel을 흰색으로 clear
}
}
 
/* 직선을 그리는 함수 */ 
void draw_line(int x1, int y1, int x2, int y2, short int line_color){
int t;  // 임시 변수 지정
int st = (abs(y2-y1)>abs(x2-x1));  // 기울기를 비교하는 변수 지정  
if(st){  // 기울기의 절대값이 1보다 크면
t=x1;  // x와 y를 교환하여 기울기를 1보다 작게 한다.
x1=y1;  // …
y1=t;
t=x2;
x2=y2;
y2=t;
}
if(x1>x2){  // x1이 x2보다 크면
t=x1;  // x1과 x2를 교환한다.
x1=x2;  // ...
x2=t;  // …
t=y1;  // y1과 y2를 교환한다.
y1=y2;  // …
y2=t;  // …
}
int dx = abs(x1 - x2); // x의 간격을 나타내는 변수 지정
     int dy = abs(y1 - y2); // y의 간격을 나타내는 변수 지정
 
int error = -(dx/2); // x가 한 pixel움직일때마다 y가 이동할지를 결정할 error값
int y = y1; // 변수 y는 y1부터 시작
 
int i; // 변수 y의 증감량
if(y1<y2) i=1; // y2가 y1보다 크면 1
else i=-1; // 아니면 -1씩 이동
 
int x; // 변수 x
for(x=x1;x<=x2;x++){ // 변수 x는 1부터 시작하여 1씩 증가시키면서
if(st) plot_pixel(y, x, line_color); // 기울기가 1보다 크면 x, y를 다시 원래대로 교환 
else plot_pixel(x, y, line_color); // 작으면 그대로 색값을 넣는다.
error = error + dy; // error에서 dy를 더 하고
if(error>=0){ // error가 0보다 크거나 같으면 현재 x, y의 기울기가 그리려는 기울기보다 1/2이하이므로 
y=y+i; // y를 증가 또는 감소 시켜 그리려는 기울기를 따라간다.
error = error - dx; // -dx를 하여 error를 재설정
}
}
}
 
int main(void){
pixel_buffer_start = *pixel_ctrl_ptr;  // 버퍼에 그린다.
 
clear_screen ();
draw_line(0, 0, 150, 150, 0x001F);  // 파란색 선
draw_line(150, 150, 319, 0, 0x07E0);  // 초록색 선
draw_line(0, 239, 319, 239, 0xF800);  // 빨간색 선
draw_line(319, 0, 0, 239, 0xF81F);  // 분홍색 선
}
 