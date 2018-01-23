#include <stdio.h>

#define clears() printf("\033[H\033[J")
#define gotos(x,y) printf("\033[%d;%dH", (x), (y))

#define WIDTH 11
#define HEIGHT 10

char screen[WIDTH * HEIGHT];

void screen_fill(char c);

void screen_clear();

void screen_print();

int main()
{
    screen_fill('#');

    screen_print();
    return 0;
}

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
