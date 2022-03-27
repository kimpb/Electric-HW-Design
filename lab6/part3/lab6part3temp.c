#include <stdio.h>
#include "address_map_nios2.h"


static volatile int *HEX3_HEX0_ptr = (int*) HEX3_HEX0_BASE;
static volatile int *HEX5_HEX4_ptr = (int*) HEX5_HEX4_BASE;

int SEG7_CODE(int i){
    char BIT_CODE[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};
        return BIT_CODE[i];
}

void display_HEX3_0(unsigned int a){
    unsigned int b;
    unsigned int c;
    b = SEG7_CODE(a%10);
    c = b;

    b = SEG7_CODE((a%100)/10);
    c = c | (b<<8);

    b = SEG7_CODE((a%1000)/100);
    c = c | (b<<16);

    b = SEG7_CODE(a/1000);
    c = c | (b<<24);

    *HEX3_HEX0_ptr = c; 
}

void display_HEX5_4(unsigned int a){
    unsigned int b;
    unsigned int c;

    b = SEG7_CODE(a%10);
    c = SEG7_CODE(a/10);
    c = b | (c<<8);
    *HEX5_HEX4_ptr = c;
}

int longest1(unsigned int TEST_NUM){
    unsigned int b, c;
    int count;
    count = 0;
    b = TEST_NUM;
    while(b != 0){
        c = b >> 1;
        b = b & c;
        count++;
    }
    return count;
}

int longestALT(unsigned int TEST_NUM){
    int result0, result1;
    result0 = longest1(TEST_NUM ^ 0x55555555);
    result1 = longest1(TEST_NUM ^ 0xAAAAAAAA);
    if(result0>result1){
        return result0;
    }
    else{
        return result1;
    }
}

int main(){
    unsigned int TEST_NUM[] = {0x0000e000, 0x3fabedef, 0x00000001, 0x00000002, 0x75a5a5a5,
    0x01ffC000, 0x03ffc000, 0x55555555, 0x77777777, 0x88888888, 0x00000000};
    int t0, ex0, t1, ex1, tALT, exALT;
    int i;
    ex0 = 0; ex1 = 0; exALT = 0;
    for(i=0;i<11;i++){
        if(TEST_NUM[i] == 0){
            printf("break");
            break;
        }

        t0 = longest1(~TEST_NUM[i]);
        if(t0<ex0)
        t0 = ex0;
        else
        ex0 = t0;
        printf("%d, ", t0);
        t1 = longest1(TEST_NUM[i]);
        if(t1<ex1)
        t1 = ex1;
        else
        ex1 = t1;
        printf("%d, ", t1);
        tALT = longestALT(TEST_NUM[i]);
        if(tALT<exALT)
        tALT = exALT;
        else
        exALT = tALT;
        printf("%d, ", tALT);
        }
    
    display_HEX3_0(t0*100 + t1);
    display_HEX5_4(tALT);
    
    return 0;
}




