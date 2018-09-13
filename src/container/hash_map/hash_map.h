#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdint.h>
#include <stdlib.h>

struct hash_map_node_t {
    char *key;
    void *data;
    struct hash_map_node_t *next;
};

struct hash_map_t {
    struct hash_map_node_t **arr;
    size_t size;       //filled size
    size_t len;        //size of array
};

void hash_map_init(struct hash_map_t *this, size_t len);

size_t hash_map_size(struct hash_map_t *this);

void hash_map_delete(struct hash_map_t *this);
void hash_map_delete_all(struct hash_map_t *this);

void hash_map_insert(struct hash_map_t *this, char *key, void *data);

void *hash_map_get(struct hash_map_t *this, char *key);

void *hash_map_remove(struct hash_map_t *this, char *key);

void *hash_map_for_each(struct hash_map_t *this, void *(*fun)(char *key, void *data, void *ref), void *ref);

#endif
