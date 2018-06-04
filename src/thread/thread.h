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
    int type;
#ifdef _WIN32
    HANDLE *handle;
    DWORD thread_id; 
#else
    struct pthread_t *handle;
#endif
};

int thread_create(struct thread *this, int (*fun)(void *arg), void *arg);

int thread_join(struct thread *this);

#endif


