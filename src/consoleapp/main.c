#include <stdio.h>

#define SCREEN_C
#include "screen.c"
#undef SCREEN_C

int main()
{
    screen_clear();
    screen_set_border();

    screen_set_char_color(1,1, 'h', 0, 255);
    screen_set_char_color(2,2, 'h', 50, 50);
    screen_set_char_color(3,3, 'h', 0, 0);
    screen_set_char_color(4,4, 'h', 5, 2);
    screen_set_char_color(5,5, 'h', COLOR_G, COLOR_B);


    screen_terminate();
    return 0;
}


