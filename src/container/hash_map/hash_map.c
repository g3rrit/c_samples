#include "hash_map.h"

#include <stdint.h>
#include <string.h>

size_t get_position(char *name, size_t max_size) {
    size_t s_t_s = sizeof(size_t);
    uint8_t res_hash[s_t_s];
    memset(res_hash, 0, s_t_s);
    int hash_pos = 0;
    int name_len = strlen(name);

    for(int i = 0; i < name_len; i++) {
        res_hash[hash_pos] ^= (uint8_t)name[i];
        hash_pos++;
        hash_pos %= s_t_s - 1; 
    }

    uint32_t res = 0;
    memcpy(&res, res_hash, s_t_s);

    return res % max_size;
}

void hash_map_init(struct hash_map_t *this, size_t len) {
    this->len = len;
    this->size = 0;

    this->arr = malloc(sizeof(struct hash_map_node_t*) * len);
    if(!this->arr) {
        printf("error -> allocation\n");
        exit(-1);
    }

    memset(this->arr, 0, sizeof(struct hash_map_node_t*) * len);
}

size_t hash_map_size(struct hash_map_t *this) {
    return this->size;
}

void hash_map_delete(struct hash_map_t *this) {
    struct hash_map_node_t *entry = 0;
    struct hash_map_node_t *next = 0;

    for(int i = 0; i < this->len; i++) {
        entry = this->arr[i];

        while(entry) {
            next = entry->next;
            free(entry);
            entry = next;
        }
    }

    this->size = 0;
    this->len = 0;
}

void hash_map_delete_all(struct hash_map_t *this) {
    struct hash_map_node_t *entry = 0;
    struct hash_map_node_t *next = 0;

    for(int i = 0; i < this->len; i++) {
        entry = this->arr[i];

        while(entry) {
            next = entry->next;
            free(entry->data);
            free(entry);
            entry = next;
        }
    }

    this->size = 0;
    this->len = 0;
}

void hash_map_insert(struct hash_map_t *this, char *key, void *data) {
    struct hash_map_node_t *node = malloc(sizeof(struct hash_map_node_t));
    if(!node) {
        printf("error -> allocation\n");
        exit(-1);
    }

    *node = (struct hash_map_node_t) {
        .key = key,
        .data = data,
        .next = 0
    };

    struct hash_map_node_t **entry = &(this->arr[get_position(key, this->len)]);

    while(*entry)
        entry = &(*entry)->next;

    *entry = node;

    this->size++;
}

void *hash_map_get(struct hash_map_t *this, char *key) {
    struct hash_map_node_t **entry = &(this->arr[get_position(key, this->len)]);

    if(!*entry)
        return 0;

    while(*entry && strcmp((*entry)->key, key))
        entry = &(*entry)->next;

    if(!*entry)
        return 0;

    return (*entry)->data;
}

void *hash_map_remove(struct hash_map_t *this, char *key) {
    struct hash_map_node_t **entry = &(this->arr[get_position(key, this->len)]);

    if(!*entry)
        return 0;

    while(*entry && strcmp((*entry)->key, key))
        entry = &(*entry)->next;

    if(!*entry)
        return 0;

    struct hash_map_node_t *f_node = *entry;
    void *data = f_node->data;

    *entry = (*entry)->next;

    free(f_node);

    this->size--;

    return data;
}

void *hash_map_for_each(struct hash_map_t *this, void *(*fun)(char *key, void *data, void *ref), void *ref) {
    struct hash_map_node_t *entry = 0;
    struct hash_map_node_t *next = 0;

    for(int i = 0; i < this->len; i++) {
        entry = this->arr[i];

        while(entry) {
            next = entry->next;

            void *ret = fun(entry->key, entry->data, ref);
            if(ret)
                return ret;

            entry = next;
        }
    }

    return 0;
}



