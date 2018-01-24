//hdr
#ifndef SCREEN_H
#define SCREEN_H

#include "stdio.h"

#define clears() printf("\033[H\033[J")
#define gotos(x,y) printf("\033[%d;%dH", (y+1), (x+1))

#define WIDTH 20
#define HEIGHT 10
#define BORDER_V '|'
#define BORDER_H '-'
#define BORDER_C '+'

#define COLOR_BL 0
#define COLOR_R 1
#define COLOR_G 2
#define COLOR_Y 11
#define COLOR_B 12
#define COLOR_W 15

//WIDTH at the end there is a '\n'

//char screen[WIDTH * HEIGHT];

//char screen_colors[WIDTH * HEIGHT * 3];

void screen_fill_char(char c);

void screen_clear();

void screen_set_char(int x, int y, char c);

void screen_set_char_color(int x, int y, char c, char color_v, char color_b);

void screen_set_border();

void screen_terminate();

#endif

//src
#ifndef SCREEN_C

#define set_color_v(c) printf("\033[38;5;%dm",(c))
#define set_color_b(c) printf("\033[48;5;%dm",(c))
#define clear_color() printf("\033[0m")

void screen_clear()
{
    screen_fill_char('h');
}

void screen_fill_char(char c)
{
    clears();
    gotos(0,0);
    char fill_line[WIDTH + 1];
    for(int i = 0; i < WIDTH; fill_line[i++] = c);
    fill_line[WIDTH] = 0;

    for(int y = 0; y < HEIGHT; y++)
    {
        printf("%s\n", fill_line);
    }
}

void screen_set_char_color(int x, int y, char c, char color_v, char color_b)
{
    if(x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        return;

    gotos(x,y);
    set_color_v(color_v);
    set_color_b(color_b);
    //set_color_b(b_r,b_g,b_b);
    printf("%c", c);
    clear_color();
}

void screen_set_border()
{
    char fill_line[WIDTH-1];
    for(int i = 0; i < WIDTH; fill_line[i++] = BORDER_H);
    fill_line[WIDTH-2] = 0;

    //fill top
    gotos(1,0);
    printf("%s", fill_line);

    //fill bottom
    gotos(1,HEIGHT-1);
    printf("%s", fill_line);

    //fill left
    for(int i = 1; i < HEIGHT - 1; i++)
    {
        gotos(0,i);
        printf("%c", BORDER_V);
    }

    //fill right
    for(int i = 1; i < HEIGHT - 1; i++)
    {
        gotos(WIDTH-1,i);
        printf("%c", BORDER_V);
    }

    //fill edges
#define border_print() printf("%c", BORDER_C)
    gotos(0,0);
    border_print();
    gotos(WIDTH-1,0);
    border_print();
    gotos(0,HEIGHT-1);
    border_print();
    gotos(WIDTH-1,HEIGHT-1);
    border_print();
#undef border_print

}

void screen_terminate()
{
    gotos(WIDTH-1,HEIGHT-1);
    printf("\n");
}

#endif

