#include "error.h"
#include "stdio.h"

void loo_err_h(int *res)
{
    *res = 1;
}

result loo()
{
    err("loo error", &loo_err_h);
    int res = 0;
    ok(res);
}

void foo_err_h(int *res)
{
    *res = 2;
}

result foo() 
{
    int res = 0;
    try(res, loo(), "loo error in foo", &foo_err_h);
    ok(res);
}

void bar_err_h(int *res)
{
    *res = 3;
}

result bar() 
{
    int res = 0;
    try(res, foo(), "foo error in bar", &bar_err_h);
    ok(res);
}

int main()
{
    int res = 0;
    catch(res, bar(), {
        int res = 0;
        err_h(&res);
        printf("error msg: %s res: %i\n", msg, res);
    });            
    return 0;
}

