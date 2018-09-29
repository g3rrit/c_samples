#ifndef HASH_TREE_H
#define HASH_TREE_H

#include "stdlib.h"
#include "stdarg.h"

#include "hash_map.h"

struct hash_tree_node_t {
    void *data;
    struct hash_map_t *tree;
}

struct hash_tree_t {
    struct hash_map_t root; 
    size_t size;
    size_t map_len;
};

void hash_tree_init(struct hash_tree_t *this, size_t map_len);

void hash_tree_delete(struct hash_tree_t *this);

void hash_tree_delete_all(struct hash_tree_t *this);

size_t hash_tree_size(struct hash_tree_t *this);

//keys follow as char*
//key list is terminated with 0
//returns 0 on success otherwise element that is already present
void *hash_tree_insert(struct hash_tree_t *this, void *data,  ...);

void *hash_tree_get(struct hash_tree_t *this, ...);

void *hash_tree_remove(struct hash_tree_t *this, ...);

void *hash_tree_for_each(struct hash_tree_t *this, void *(*fun)(void *data, void *ref, ...), void *ref);

#endif
