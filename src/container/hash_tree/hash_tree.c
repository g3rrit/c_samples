#include "hash_tree.h"

#include <stdlib.h>

void hash_tree_init(struct hash_tree_t *this, size_t map_len) {
    hash_map_init(&this->root, map_len);
    this->map_len = map_len;
    this->size = 0;
}

void hash_tree_delete(struct hash_tree_t *this) {

}

void hash_tree_delete_all(struct hash_tree_t *this) {
}

size_t hash_tree_size(struct hash_tree_t *this) {
    return this->size;
}

void *hash_tree_insert(struct hash_tree_t *this, void *data,  ...) {
    va_list args;
    va_start(args, data);

    char *key = va_arg(args, char*);
    struct hash_tree_node_t *entry = hash_map_get(&this->root, key);
    key = va_arg(args, char*);

    while(key) {
        if(!entry) {
            hash_map_insert(
        }
    }


    va_end(args);
}

void *hash_tree_get(struct hash_tree_t *this, ...);

void *hash_tree_remove(struct hash_tree_t *this, ...);

void *hash_tree_for_each(struct hash_tree_t *this, void *(*fun)(void *data, void *ref, ...), void *ref);


