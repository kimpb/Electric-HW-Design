
#include <stdio.h>
#include "address_map_nios2.h"

static volatile int *HEX3_HEX0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_HEX4_ptr = (int*) HEX5_HEX4_BASE; // HEX3_0, HEX5_4 포인터 지정

// 0~9까지의 정수를 넘겨받아 그 수에 해당하는 7segment code를 return하는 function
int SEG7_CODE(int i){
    char BIT_CODE[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111}; // 0~9까지의 7segment
        return BIT_CODE[i];
}

// int a를 HEX3_0에 display 하는 function
void display_HEX3_0(unsigned int a){
    unsigned int b;
    unsigned int c;
    b = SEG7_CODE(a%10); // b에는 a의 1의 자리에 해당하는 7segment code 저장
    c = b; // c에 b를 저장

    b = SEG7_CODE((a%100)/10); // b에는 a의 10에 자리에 해당하는 7segment code 저장
    c = c | (b<<8); // 원래 c에 저장 되어있던 값과 b를 왼쪽으로 8칸 shift 시킨 값과 or 하여 합침

    b = SEG7_CODE((a%1000)/100); // b에는 a의 100에 자리에 해당하는 7segment code 저장
    c = c | (b<<16); // 원래 c에 저장 되어있던 값과 b를 왼쪽으로 8칸 shift 시킨 값과 or 하여 합침

    b = SEG7_CODE(a/1000); // b에는 a의 100에 자리에 해당하는 7segment code 저장
    c = c | (b<<24); // 원래 c에 저장 되어있던 값과 b를 왼쪽으로 8칸 shift 시킨 값과 or 하여 합침

    *HEX3_HEX0_ptr = c; // HEX3_0 포인터에 c를 저장하여 HEX3_0에 c값 출력
}

// int a를 HEX5_4에 display하는 function
void display_HEX5_4(unsigned int a){
    unsigned int b;
    unsigned int c;

    b = SEG7_CODE(a%10);
    c = SEG7_CODE(a/10); // b에는 a의 1의자리, c에는 10의자리 수에 해당하는 7segment code 저장
    c = b | (c<<8); // c에 b와 c를 왼쪽으로 8칸 shift 시킨 값을 합쳐서 저장
    *HEX5_HEX4_ptr = c; // HEX5_4 포인터에 c를 저장하여 HEX5_4에 c값 출력
}

// 가장 긴 1의 개수를 찾는 function
int longest1(unsigned int TEST_NUM){
    unsigned int b, c;
    int count; // 연속되는 1의 개수 count하는 변수
    count = 0; // count값 초기화
    b = TEST_NUM; // b <= TEST_NUM
    while(b != 0){ 
        c = b >> 1; // c에 b를 왼쪽으로 한 칸 shift 시킨 값 저장
        b = b & c; // b와 c를 and연산하여 b에 저장
        count++; // count
    } // b가 0이면, break하고 count를 return
    return count; // 연속되는 1의 개수 return
}

// 가장 긴 10 또는 01의 개수를 찾는 함수
int longestALT(unsigned int TEST_NUM){
    int result5, resultA; 
    result5 = longest1(TEST_NUM ^ 0x55555555); // result5에는 0x55555555와 xor하여 연속되는 1의 개수 count한 값 저장
    resultA = longest1(TEST_NUM ^ 0xAAAAAAAA); // resultA에는 0xAAAAAAAA와 xor하여 연속되는 1의 개수 count한 값 저장
    if(result5>resultA){
        return result5;
    } // result5가 resultA보다 크면, result5 return
    else{
        return resultA; // 그렇지 않으면, resultA return
    }
}

int main(){
    unsigned int TEST_NUM[] = {0x0000e000, 0x3fabedef, 0x00000001, 0x00000002, 0x75a5a5a5,
    0x01ffC000, 0x03ffc000, 0x55555555, 0x77777777, 0x88888888, 0x00000000}; // TEST_NUM array
    int t0, ex0, t1, ex1, tALT, exALT; // t0, t1, tALT = 현재 TEST_NUM의 가장 긴 0, 1, 01 or 10의 개수, ex0, ex1, exALT = 이전까지 가장 긴 0, 1, 01 or 10의 개수
    int i;
    ex0 = 0; ex1 = 0; exALT = 0; // 이전 값들을 저장하기 위한 변수들 초기화
    for(i=0;i<11;i++){
        if(TEST_NUM[i] == 0){
            break; 
        } // 불러온 TEST_NUM이 0이면, break

        t0 = longest1(~TEST_NUM[i]); // 연속된 0의 개수를 찾기 위해 TEST_NUM을 inversion한 후 연속된 1의 개수 저장
        if(t0<ex0)
        t0 = ex0; // 이전 값이 현재 값보다 크면 현재 값에 이전 값 저장
        else
        ex0 = t0; // 현재 값이 이전 값보다 크면 이전 값에 현재 값 저장
        t1 = longest1(TEST_NUM[i]); // 연속된 1의 개수 찾는 함수 호출
        if(t1<ex1)
        t1 = ex1; // 이전 값이 현재 값보다 크면 현재 값에 이전 값 저장
        else
        ex1 = t1; // 현재 값이 이전 값보다 크면 이전 값에 현재 값 저장
        tALT = longestALT(TEST_NUM[i]); // 연속된 10 또는 01의 개수 찾는 함수 호출
        if(tALT<exALT)
        tALT = exALT; // 이전 값이 현재 값보다 크면 현재 값에 이전 값 저장
        else
        exALT = tALT; // 현재 값이 이전 값보다 크면 이전 값에 현재 값 저장
        }
    
    display_HEX3_0(t0*100 + t1);
    display_HEX5_4(tALT);
    // HEX3_0에 (t0, t1), HEX5_4에 (tALT) 출력 후 종료
    return 0;
}