#define LEDR_BASE 0xFF200000

static volatile int *LEDR_ptr = (int*) LEDR_BASE;


int main(){
    int LIST[8] = {7, 4, 5, 3, 6, 1, 8, 2};
    int i;
    int max = 0;
    for(i = 0; i <= LIST[0]; i++)
    {
        if(max<LIST[i])
        max = LIST[i];
    }
    *LEDR_ptr = max;
}