#include <stdio.h>

#define SCREEN_C
#include "screen.c"
#undef SCREEN_C

int main()
{
    screen_fill('o');
    screen_set_border();

    screen_print();
    return 0;
}


