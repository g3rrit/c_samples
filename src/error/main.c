#include "error.h"
#include "stdio.h"

result loo()
{
    err("loo error", 1);
    int res = 0;
    ok(res);
}

result foo() 
{
    int res = 0;
    try(res, loo(), "loo error in foo", 2);
    ok(res);
}

result bar() 
{
    int res = 0;
    try(res, foo(), "foo error in bar", 3);
    ok(res);
}

int main()
{
    int res = 0;
    catch(0, bar(), {
        printf("error msg: %s res: %i\n", msg, err);
    });            
    return 0;
}

