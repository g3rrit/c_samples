//head
#ifndef BAR_H
#define BAR_H

char *bar();

#endif

//src

#ifndef BAR_C

#include <string.h>
#include <stdlib.h>

char *bar()
{
    char *res = malloc(sizeof(char) * 4);

    strcpy(res, "abc");
    res[3] = 0;

    return res;
}

#endif

