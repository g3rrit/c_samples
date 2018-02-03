
#include <stdio.h>

struct foo
{
    struct foo *data;
    int value;
};

int main()
{
    struct foo **foo_b;
    *foo_b = malloc(sizeof(struct foo));
    struct foo *foo_d = malloc(sizeof(struct foo)); 
    (*foo_b)->data = foo_d;
    foo_d->value = 10;

    free(*foo_b);

    printf("foo_d->value: %i\n", foo_d->value);
}
