#include "array.h"

#include <stdio.h>

void array_grow(struct array_t *this) {
    this->len = (3 * this->len) / 2;
    this->data = realloc(this->data, this->len * sizeof(void*));
    if(!this->data) {
        printf("realloc of array failed!");
        exit(-1);
    }
}

void array_shrink(struct array_t *this) {
    this->len = (2 * this->len) / 3;
    this->data = realloc(this->data, this->len * sizeof(void*));
    if(!this->data) {
        printf("realloc of array failed!");
        exit(-1);
    }
}

void array_init(struct array_t *this) {
    this->size = 0;
    this->len = 16;
    this->data = malloc(this->len * sizeof(void*));
}

void array_insert(struct array_t *this, void *data) {
    if(!this) {
        return;
    }

    if(this->size >= this->len) {
        array_grow(this);
    }

    this->data[this->size] = data;
    this->size++;
}

void *array_remove(struct array_t *this, size_t pos) {
    if(!this) {
        return 0;
    }

    if(this->size <= this->len/2 && this->len > 16) {
        array_shrink(this);
    }
}

void *array_at(struct array_t *this, size_t pos) {
    if(!this) {
        return 0;
    }

    if(pos >= this->size) {
        return 0;
    }

    return this->data[pos];
}

size_t array_size(struct array_t *this) {
    if(!this) {
        return 0;
    }

    return this->size;
}

void array_for_each(struct array_t *this, void (*fun)(void *data, void *ref), void *ref) {
    if(!this) {
        return;
    }

    for(size_t i = 0; i < this->size; i++) {
        fun(this->data[i], ref);
    }
}

void array_call(struct array_t *this, void (*fun)(void *data)) {
    if(!this) {
        return;
    }

    for(size_t i = 0; i < this->size; i++) {
        fun(this->data[i]);
    }
}

void array_delete(struct array_t *this) {
    if(!this) {
        return;
    }

    free(this->data);
    this->data = 0;
    this->size = 0;
    this->len = 0;
}


