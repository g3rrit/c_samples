#include <stdio.h>

#define THREAD_C
#include "thread.c"
#undef THREAD_C

static void *foo(void *val)
{
    int i = 0;
    while(++i <= 1000)
    {
        printf("A -------- printing: %i\n", i);
    }

    return i;
}

int main()
{
    //pthread_start();

    struct thread mthread;

    thread_init(&mthread);

    if(thread_create(&mthread, 0, &foo, 0))
    {
        printf("failed to create thread\n");
    }

    int i = 0;
    while(++i <= 1000)
    {
        printf("B -------- printing: %i\n", i);
    }

    int n = 0;
    thread_join(&mthread, &n);
    printf("thread joined with %i\n", n);
}
