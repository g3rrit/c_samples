//hdr
//compile with -lpthread
//thread wrapper file
#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

struct thread
{
    struct pthread_t *pthread;
};

void thread_init(struct thread *this);

int thread_create(struct thread *_thread, const pthread_attr_t *attr, void *(*fun)(void *), void *arg);

int thread_join(struct thread *_thread, void **ret);

#endif

//src
#ifndef THREAD_C

void thread_init(struct thread *this)
{
    this->pthread = malloc(sizeof(struct thread));
}

inline int thread_create(struct thread *_thread, const pthread_attr_t *attr, void *(*fun)(void *), void *arg)
{
    pthread_create(_thread->pthread, attr, fun, arg);
}

int thread_join(struct thread *_thread, void **ret)
{
    pthread_join(_thread, ret);
    free(_thread->pthread);
}

#endif

