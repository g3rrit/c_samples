//head
#ifndef FOO_H
#define FOO_H

void foo();

#endif

//src
#ifndef FOO_C

#define BAR_C
#include "bar.c"
#undef BAR_C

void foo()
{
    printf("headerless file working\n");
    printf("bar: %s\n", bar());
}

#endif
