#ifndef VECTOR_H
#define VECTOR_H

struct vector_t
{
    void **data;
    int size;
    int allocated_size;
    int size_of_element;
};

struct vector_info
{
    struct vector_t *this;
    int pos;
    int size;
};

void vector_init(struct vector_t *this, int size_of_element, int size);

int vector_resize(struct vector_t *this, int size);

void *vector_push_front(struct vector_t *this, void *data);
void *vector_push_back(struct vector_t *this, void *data);

void *vector_pop_front(struct vector_t *this);
void *vector_pop_back(struct vector_t *this);

void *vector_at(struct vector_t *this, int x);

int vector_set(struct vector_t *this, int x, void *data);

void vector_delete(struct vector_t *this);

void vector_delete_all(struct vector_t *this);

void *vector_remove_at(struct vector_t *this, int x);

void *vector_remove_at_unordered(struct vector_t *this, int x);

void *vector_for_each(struct vector_t *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref);

//------------------------- 

#endif
