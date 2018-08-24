#include "vector.h"

#include <stdlib.h>

#define test_resize() if(this->size >= this->allocated_size) vector_resize(this, this->size + 10)
#define test_shrink() if(this->size <= this->allocated_size - 20) vector_resize(this, this->size + 10)

void vector_init(struct vector_t *this, int size_of_element, int size)
{
    this->size = 0;
    this->size_of_element = size_of_element;
    this->data = malloc(sizeof(void*) * size);
    this->allocated_size = size;
    for(int i = 0; i < size; i++)
        this->data[i] = 0;
}

int vector_resize(struct vector_t *this, int size)
{
    if(this->size >= size)
        return 0;

    //realloc data 
    this->data = realloc(this->data, sizeof(void*) * size);

    this->allocated_size = size;
    for(int i = this->size; i < this->allocated_size; i++)
        this->data[i] = 0;

    if(!this->data)
        return 0;
    else
        return 1;
}

void *vector_push_front(struct vector_t *this, void *data)
{
    test_resize();

    for(int i = this->size; i >= 0; i--)
        this->data[i] = this->data[i-1];          

    this->size++;

    this->data[0] = data;

    return data;
}

void *vector_push_back(struct vector_t *this, void *data)
{
    test_resize();

    this->data[this->size] = data;
    this->size++;

    return data;
}

void *vector_pop_front(struct vector_t *this)
{
    void *res = this->data[0];

    for(int i = 0; i < this->size - 1; i++)
        this->data[i] = this->data[i+1];

    this->data[this->size-1] = 0;

    this->size--;

    test_shrink();

    return res;
}

void *vector_pop_back(struct vector_t *this)
{
    void *data = this->data[this->size-1];
    this->data[this->size - 1] = 0;
    this->size--;

    test_shrink();

    return data;
}

void *vector_at(struct vector_t *this, int x)
{
    return this->data[x];
}

int vector_set(struct vector_t *this, int x, void *data)
{
    if(this->allocated_size <= x)
       vector_resize(this, x + 10);

    if(this->size <= x)
        this->size = x+1;

    this->data[x] = data;
    return 1;
}

void vector_delete(struct vector_t *this)
{
    if(this->data)
        free(this->data);
    this->size = 0;
    this->allocated_size = 0;
    this->data = 0;
    this->size_of_element = 0;
}

void vector_delete_all(struct vector_t *this)
{
    for(int i = 0; i < this->size; i++)
    {
        free(this->data[i]);
    }
    vector_delete(this);
}

//unordered
void *vector_remove_at(struct vector_t *this, int x)
{
    void *res = this->data[x]; 

    for(; x < this->size - 1; x++)
        this->data[x] = this->data[x+1];

    this->data[this->size - 1] = 0;

    this->size--;

    test_shrink();

    return res;
}

//ordered
void *vector_remove_at_unordered(struct vector_t *this, int x)
{
    if(x >= this->size)
        return 0;

    void *res = this->data[x];
    this->data[x] = this->data[this->size - 1];
    this->data[this->size - 1] = 0;
    this->size--;

    test_shrink();

    return res;
}

void *vector_for_each(struct vector_t *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref)
{
    
    struct vector_info info;
    info.this = this;
    info.pos = 0;
    info.size = this->size;

    void *retval = 0;
    for(int i = 0; i < this->size; i++)
    {
        if((retval = fun(this->data[i], ref, &info)))
            return retval;
        info.pos++;
    }
    return 0;
}
