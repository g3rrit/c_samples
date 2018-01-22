//head
#ifndef FOO_H
#define FOO_H

void foo();

#endif

//src
#ifndef INCLUDE

#define INCLUDE

#include "bar.c"

#undef INCLUDE

void foo()
{
    printf("headerless file working\n");
    printf("bar: %s\n", bar());
}

#endif
