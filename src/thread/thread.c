//hdr
//compile with -lpthread
//thread wrapper file
#ifndef THREAD_H
#define THREAD_H

#ifdef _WIN32
#include "windows.h"
#else
#include "pthread.h"
#endif

struct thread
{
#ifdef _WIN32
    HANDLE *handle;
    DWORD thread_id; 
#else
    struct pthread_t *handle;
#endif
};

void thread_init(struct thread *this);

int thread_create(struct thread *_thread, void *attr, void *(*fun)(void *), void *arg);

int thread_join(struct thread *_thread, void **ret);

#endif

//src
#ifndef THREAD_C

void thread_init(struct thread *this)
{
#ifdef _WIN32 
    this->handle = malloc(sizeof(HANDLE));
#else
    this->pthread = malloc(sizeof(struct thread));
#endif
}

inline int thread_create(struct thread *_thread, void *attr, void *(*fun)(void *), void *arg)
{
#ifdef _WIN32
    CreateThread(0, 0, fun, arg, 0, &(_thread->thread_id));
#else
    pthread_create(_thread->handle, attr, fun, arg);
#endif
}

int thread_join(struct thread *_thread, void **ret)
{
#ifdef _WIN32
    WaitForSingleObject(*(_thread->handle), INFINITE);
#else
    pthread_join(_thread, ret);
#endif
    free(_thread->handle);
}

#endif

