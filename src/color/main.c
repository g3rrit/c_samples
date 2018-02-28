#define COLOR_C
#include "color.c"
#undef COLOR_C

int main()
{
    struct color c1 = { 10, 2, 3 ,5 };

    printf("sizeof color is: %i\n", sizeof(c1));

    return 0;
}
