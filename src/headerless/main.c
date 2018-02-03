#include <stdio.h>

#define FOO_C
#include "foo.c"
#undef FOO_C

#define LOG_C
#include "log.c"
#undef LOG_C


int main()
{
    foo();
    logi();
    return 0;
}
