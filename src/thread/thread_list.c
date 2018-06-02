#include "thread_list.h"

#include "error.h"
#include "stdlib.h"

int thread_list_init(struct thread_list *this)
{
    mutex_init(this->list_mutex);

    this->array = malloc(sizeof(void*) * MAX_LIST_SIZE);
    memset(this->array, 0, sizeof(void*) * MAX_LIST_SIZE);
    this->size = 0;

    return 1;
}

int thread_list_delete(struct thread_list *this)
{
    mutex_delete(this->list_mutex);

    free(this->array);
    this->size = 0;

    return 1;
}

//returns location of element
int thread_list_add(struct thread_list *this, void *data)
{
    mutex_lock(this->list_mutex);

    int pos;
    for(pos = 0; this->array[pos] && pos < MAX_LIST_SIZE; pos++);

    if(pos >= MAX_LIST_SIZE)
    {
        set_error(400);
        mutex_unlock(this->list_mutex);
        return 0;
    } 

    this->size++;

    this->array[pos] = data;

    mutex_unlock(this->list_mutex);

    return pos;
}

//returns pointer to element
void *thread_list_remove(struct thread_list *this, int pos)
{
    mutex_lock(this->list_mutex);

    if(pos >= MAX_LIST_SIZE || pos < 0)
    {
        set_error(401);
        mutex_unlock(this->list_mutex);
        return 0;
    }

    void *el = this->array[pos];

    if(this->array[pos])
        this->size--;

    this->array[pos] = 0;

    mutex_unlock(this->list_mutex);

    return el;
}

void *thread_list_get(struct thread_list *this, int pos)
{
    mutex_lock(this->list_mutex);

    if(pos >= MAX_LIST_SIZE || pos < 0)
    {
        set_error(401);
        mutex_unlock(this->list_mutex);
        return 0;
    }

    void *el = this->array[pos];

    mutex_unlock(this->list_mutex);

    return el;
}

void *thread_list_for_each(struct thread_list *this, void *(*fun)(struct thread_list *list, void *element, int pos, void *ref), void *ref)
{
    mutex_lock(this->list_mutex);

    void *retval = 0;

    for(int i = 0; i < MAX_LIST_SIZE; i++)
    {
        if(this->array[i])
        {
            retval = fun(this, this->array[i], i, ref);
            if(retval)
                return retval;
        }
    }

    mutex_unlock(this->list_mutex);
}


