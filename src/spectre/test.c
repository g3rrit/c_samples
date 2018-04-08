
#include "stdio.h"

int main()
{
    char arr[1000];
    for(int i = 0; i < 1000; i++)
        arr[i] = 1;

    printf("address of array: %p\n", arr);
    getchar();
}
