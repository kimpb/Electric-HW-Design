
#include <stdio.h>

int main(){
    int LIST[8] = {7, 4, 5, 3, 6, 1, 8, 2};
    int size;
    int i;
    int large;
    large = 0;
    size = sizeof(LIST) / sizeof(int);
    for(i = 0; i < size; i++)
    {
        if(large<LIST[i])
        large = LIST[i];
    }
    printf("Largest number = %d", large);
    return 0;
}