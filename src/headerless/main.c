#include <stdio.h>

#define INCLUDE

//when including headerless files #define INCLUDE and #undef
#include "foo.c"

#undef INCLUDE

int main()
{
    foo();
    return 0;
}
