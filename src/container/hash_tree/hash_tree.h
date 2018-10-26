#ifndef HASH_TREE_H
#define HASH_TREE_H

#include "stdlib.h"

#include "hash_map.h"

struct hash_tree_node_t {
    void *data;
    struct hash_map_t *tree;
};

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
void *hash_tree_insert(struct hash_tree_t *this, char **keys, void *data);

void *hash_tree_get(struct hash_tree_t *this, char **keys);

void *hash_tree_remove(struct hash_tree_t *this, char **keys);

typedef void (*hash_tree_for_each_f)(char **keys, void *data, void *ref);

typedef void (*hash_tree_call_f)(void *data);

void hash_tree_call(struct hash_tree_t *this, void (*fun)(void *data));

void hash_tree_for_each(struct hash_tree_t *this, void (*fun)(char **keys, void *data, void *ref), void *ref);

#endif
