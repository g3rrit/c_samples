#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

/*
 * datastructure for storing
 * elements in order
 * insertion time O(1)
 * remove time O(n)
 * at time O(1)
 */
struct array_t {
    void **data;
    size_t size;
    size_t len;
};

void array_init(struct array_t *this);

void array_insert(struct array_t *this, void *data);

void *array_remove(struct array_t *this, size_t pos);

void *array_at(struct array_t *this, size_t pos);

size_t array_size(struct array_t *this);

typedef void (*array_for_each_f)(void *data, void *ref);

void array_for_each(struct array_t *this, void (*fun)(void *data, void *ref), void *ref);

typedef void (*array_call_f)(void *data);

void array_call(struct array_t *this, void (*fun)(void *data));

void array_delete(struct array_t *this);

#endif
