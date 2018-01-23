#ifndef LOG_H
#define LOG_H

#define FOO_C
#include "foo.c"
#undef FOO_C

void logi();

#endif

#ifndef LOG_C


#include "stdio.h"

void logi()
{
    printf("log\n");
}

#endif
