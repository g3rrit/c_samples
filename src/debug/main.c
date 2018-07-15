
#include "debug.h"

int foo();

int bar();

int main()
{

    foo();
    foo();

    return 0;
}

int foo()
{
    debug_log();

    for(int i = 0; i < 10; i++)
        bar();
}

int bar()
{
    debug_log();
}

