#define PROGRESSBAR_C
#include "progressbar.c"
#undef PROGRESSBAR_C

int main()
{
    printf("progressbar-test:\n");
    char c = 'd';
    double value = .01f;
    progressbar_draw("name", 100, value);
    while((c = getchar()) != 'q')
    {
        progressbar_redraw("name", 100, value);
        value += 0.05;
    }

    return 0;
}

