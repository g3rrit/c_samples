#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

#include "mutex.h"

struct thread_queue_node
{
    struct thread_queue_node *next;
    void *data;
};

struct thread_queue
{
    mutex_declare(list_mutex);
    struct thread_queue_node *head;
    struct thread_queue_node *tail;
    int size;
};

int thread_queue_init(struct thread_queue *this);

int thread_queue_delete(struct thread_queue *this);

int thread_queue_push(struct thread_queue *this, void *data);

void *thread_queue_pop(struct thread_queue *this);

int thread_queue_size(struct thread_queue *this);

#endif