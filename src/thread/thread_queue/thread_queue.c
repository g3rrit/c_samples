#include "thread_queue.h"

int thread_queue_init(struct thread_queue *this)
{
    mutex_init(this->list_mutex);
    this->size = 0;
    this->head = 0;
    this->tail = 0;
    return 1;
}

int thread_queue_delete(struct thread_queue *this)
{
    mutex_lock(this->list_mutex);

    struct thread_queue_node *entry = this->head;
    struct thread_queue_node *del;

    while(entry)
    {
        del = entry;
        entry = entry->next;
        free(del);
    }

    this->head = 0;
    this->tail = 0;
    this->size = 0;

    mutex_unlock(this->list_mutex);
    mutex_delete(this->list_mutex);
    return 1;
}

int thread_queue_push(struct thread_queue *this, void *data)
{
    struct thread_queue_node *entry = malloc(sizeof(struct thread_queue_node));
    mutex_lock(this->list_mutex);
    entry->data = data;
    entry->next = 0;

    if(!this->head)
        this->head = entry;
    
    if(this->tail)
        this->tail->next = entry;

    this->tail = entry;
    this->size++;
    mutex_unlock(this->list_mutex);

    return 1;
}

void *thread_queue_pop(struct thread_queue *this)
{
    mutex_lock(this->list_mutex);
    struct thread_queue_node *entry = this->head;

    if(this->size == 1)
    {
        this->tail = 0;
        this->head = 0;
    }
    else
    {
        this->head = this->head->next;
    }

    this->size--;
    mutex_unlock(this->list_mutex);

    return entry;
}


int thread_queue_size(struct thread_queue *this)
{
    mutex_lock(this->list_mutex);
    int res = this->size;
    mutex_unlock(this->list_mutex);
    return res;
}