#ifndef WORKER_H
#define WORKER_H

#include "thread.h"

#define MAX_WORKERS 64

struct worker
{
    int id;
    struct thread *this;
    void *arg;
    void *(*callback)(struct worker *this, void *data);
};

int worker_init();

int worker_delete();

int add_worker(int (*fun)(struct worker *this), void *arg, void *(*callback)(struct worker *this, void *data));

void worker_finished(struct worker *this);

#endif
