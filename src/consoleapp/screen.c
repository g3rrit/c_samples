//hdr
#ifndef SCREEN_H
#define SCREEN_H

#include "stdio.h"

#define clears() printf("\033[H\033[J")
#define gotos(x,y) printf("\033[%d;%dH", (x), (y))

#define WIDTH 11
#define HEIGHT 10
#define BORDER_V '|'
#define BORDER_H '-'
#define BORDER_C '+'
//WIDTH at the end there is a '\n'

char screen[WIDTH * HEIGHT];

void screen_fill(char c);

void screen_clear();

void screen_print();

void screen_set(int x, int y, char c);

void screen_set_border();

#endif

//src
#ifndef SCREEN_C

void screen_clear()
{
    screen_fill(' ');
}

void screen_fill(char c)
{
    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH-1; x++)
        {
            screen[WIDTH * y + x] = c;
        }
        screen[WIDTH * y + WIDTH -1] = '\n';
    }
}

void screen_print()
{
    clears();
    gotos(0,0);
    printf("%s", screen);
}

void screen_set(int x, int y, char c)
{
    if(x >= WIDTH - 1 || x < 0 || y >= HEIGHT || y < 0)
        return;

    screen[WIDTH * y + x] = c;
}

void screen_set_border()
{
    //top and bottom side -
    for(int n = 0; n < 2; n++)
    {
        for(int i = 1; i < WIDTH - 2; i++)
        {
            if(n % 2)
            {
                screen[i] = BORDER_H;
            }
            else
            {
                screen[WIDTH * (HEIGHT-1) + i] = BORDER_H;
            }
        }
    }
    //left and right side |
    for(int n = 0; n < 2; n++)
    {
        for(int i = 1; i < HEIGHT - 1; i++)
        {
            if(n % 2)
            {
                screen[WIDTH * i] = BORDER_V;
            }
            else
            {
                screen[WIDTH * i + WIDTH - 2] = BORDER_V;
            }
        }
    }
    //corners +
    screen[0] = BORDER_C;
    screen[WIDTH - 2] = BORDER_C;
    screen[WIDTH * (HEIGHT-1)] = BORDER_C;
    screen[WIDTH * HEIGHT - 2] = BORDER_C;
}

#endif


