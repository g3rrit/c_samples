#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdint.h>

struct hash_map_node_t {
    char *key;
    void *data;
    struct hash_map_node_t *next;
};

struct hash_map_t {
    struct hash_map_node_t *arr;
    int size;       //filled size
    int len;        //size of array
};

void hash_map_init(struct hash_map_t *this, uint32_t len);

void hash_map_delete(struct hash_map_t *this);
void hash_map_delete_all(struct hash_map_t *this);

void hash_map_insert(struct hash_map_t *this, char *key, void *data);

void *hash_map_remove(struct hash_map_t *this, char *key);

void *hash_map_for_each(struct hash_map_t *this, void *(*fun)(void *data, char *key), void *ref);

#endif
