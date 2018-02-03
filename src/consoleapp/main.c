#include "mstd.h"
#include <stdio.h>

#define SCREEN_C
#include "screen.c"
#undef SCREEN_C

#define OBJECTMANAGER_C
#include "objectmanager.c"
#undef OBJECTMANAGER_C

#include <stdlib.h>

void input_command();

int main()
{
    screen_clear();
    screen_set_border();

    screen_set_char_color(1,1, 'h', 0, 255);
    screen_set_char_color(2,2, 'h', 50, 50);
    screen_set_char_color(3,3, 'h', 0, 0);
    screen_set_char_color(4,4, 'h', 5, 2);

    screen_draw_rect(3,3, 5,3, 'O', COLOR_B, COLOR_G, false);

    char input = 0;

    int xpos = 1;
    int ypos = 1;

    system("/bin/stty raw");
    while((input = getchar()) != 'q')
    {
        screen_clear();
        screen_set_border();

        gotos(0,HEIGHT);
        printf("%c", input);

        struct object_manager tempobjm;
        object_manager_init(&tempobjm);
        object_manager_fill(&tempobjm);
        object_manager_draw(&tempobjm);


        if(input == 'l')
            xpos++;
        else if(input == ':');
            //input_command();
            
        screen_set_char_color(xpos, ypos, 'o', COLOR_B, 0);
    }
    system("/bin/stty cooked");

    screen_terminate();
    return 0;
}

void input_command()
{
    system("/bin/stty cooked");

    char inputa[11]; 
    inputa[10] = 0;

    fflush(stdout);
    fgets(inputa, 10, stdin);

    printf("\ncommand: %s", inputa);

    system("/bin/stty raw");
}


