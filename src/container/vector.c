//hdr
#ifndef VECTOR_H
#define VECTOR_H

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

//---------- VECTOR ---------- 

struct vector
{
    struct container _container;
    void **data;
    int size;
    int allocated_size;
    int size_of_element;
};

struct vector_info
{
    struct vector *this;
    int pos;
    int size;
};

void vector_init(struct vector *this, int size_of_element, int size);

int vector_resize(struct vector *this, int size);

void *vector_push_front(struct vector *this, void *data);
void *vector_push_back(struct vector *this, void *data);

void *vector_pop_front(struct vector *this);
void *vector_pop_back(struct vector *this);

void *vector_at(struct vector *this, int x);

int vector_set(struct vector *this, int x, void *data);

void vector_delete(struct vector *this);

void vector_delete_all(struct vector *this);

void *vector_remove_at(struct vector *this, int x);

void *vector_remove_at_unordered(struct vector *this, int x);

void *vector_for_each(struct vector *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref);

//------------------------- 

#endif

//src
#ifndef VECTOR_C

#include <stdlib.h>

//---------- VECTOR ---------- 

#define test_resize() if(this->size >= this->allocated_size) vector_resize(this, this->size + 10)
#define test_shrink() if(this->size <= this->allocated_size - 20) vector_resize(this, this->size + 10)

void vector_init(struct vector *this, int size_of_element, int size)
{
    //init container
    this->_container.delete = &vector_delete;
    this->_container.delete_all = &vector_delete_all;
    this->_container.push_front = &vector_push_front;
    this->_container.push_back = &vector_push_back;
    this->_container.pop_front = &vector_pop_front;
    this->_container.pop_back = &vector_pop_back;
    this->_container.at = &vector_at;
    this->_container.remove_at = &vector_remove_at;
    
    this->size = 0;
    this->size_of_element = size_of_element;
    this->data = malloc(sizeof(void*) * size);
    this->allocated_size = size;
    for(int i = 0; i < size; i++)
        this->data[i] = 0;
}

int vector_resize(struct vector *this, int size)
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

void *vector_push_front(struct vector *this, void *data)
{
    test_resize();

    for(int i = this->size; i >= 0; i--)
        this->data[i] = this->data[i-1];          

    this->size++;

    this->data[0] = data;

    return data;
}

void *vector_push_back(struct vector *this, void *data)
{
    test_resize();

    this->data[this->size] = data;
    this->size++;

    return data;
}

void *vector_pop_front(struct vector *this)
{
    void *res = this->data[0];

    for(int i = 0; i < this->size - 1; i++)
        this->data[i] = this->data[i+1];

    this->data[this->size-1] = 0;

    this->size--;

    test_shrink();

    return res;
}

void *vector_pop_back(struct vector *this)
{
    void *data = this->data[this->size-1];
    this->data[this->size - 1] = 0;
    this->size--;

    test_shrink();

    return data;
}

void *vector_at(struct vector *this, int x)
{
    return this->data[x];
}

int vector_set(struct vector *this, int x, void *data)
{
    if(this->allocated_size <= x)
       vector_resize(this, x + 10);

    if(this->size <= x)
        this->size = x+1;

    this->data[x] = data;
    return 1;
}

void vector_delete(struct vector *this)
{
    free(this->data);
    this->size = 0;
    this->allocated_size = 0;
    this->data = 0;
    this->size_of_element = 0;
}

void vector_delete_all(struct vector *this)
{
    for(int i = 0; i < this->size; i++)
    {
        free(this->data[i]);
    }
    vector_delete(this);
}

//unordered
void *vector_remove_at(struct vector *this, int x)
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
void *vector_remove_at_unordered(struct vector *this, int x)
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

void *vector_for_each(struct vector *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref)
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

//------------------------- 

#endif
