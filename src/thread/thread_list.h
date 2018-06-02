#ifndef THREAD_LIST_H
#define THREAD_LIST_H

#define MAX_LIST_SIZE   128

#include "mutex.h"

struct thread_list
{
    mutex_declare(list_mutex);

    void **array;
    int size;
};

int thread_list_init(struct thread_list *this);

int thread_list_delete(struct thread_list *this);

//returns location of element
int thread_list_add(struct thread_list *this, void *data);

//returns pointer to element
void *thread_list_remove(struct thread_list *this, int pos);

void *thread_list_get(struct thead_list *this, int pos);

void *thread_list_for_each(struct thread_list *this, void *(*fun)(struct thread_list *list, void *element, int pos, void *ref), void *ref);

#endif
