#include "hash_tree.h"

#include <stdlib.h>
#include <stdio.h>

void hash_tree_init(struct hash_tree_t *this, size_t map_len) {
    hash_map_init(&this->root, map_len);
    this->map_len = map_len;
    this->size = 0;
}

void hash_tree_for_each_delete(char *key, struct hash_tree_node_t *data, void *ref) {
    if(data->tree) {
        hash_map_for_each(data->tree, (hash_map_for_each_f)&hash_tree_for_each_delete, ref);
        hash_map_delete(data->tree);
        free(data->tree);
    }

    free(data);
}

void hash_tree_delete(struct hash_tree_t *this) {
    hash_map_for_each(&this->root, (hash_map_for_each_f)&hash_tree_for_each_delete, 0);
    hash_map_delete(&this->root);

    this->size = 0;
}

void hash_tree_for_each_delete_all(char *key, struct hash_tree_node_t *data, void *ref) {
    if(data->tree) {
        hash_map_for_each(data->tree, (hash_map_for_each_f)&hash_tree_for_each_delete_all, ref);
        hash_map_delete(data->tree);
        free(data->tree);
    }

    free(data->data);
    free(data);
}

void hash_tree_delete_all(struct hash_tree_t *this) {
    hash_map_for_each(&this->root, (hash_map_for_each_f)&hash_tree_for_each_delete_all, 0);
    hash_map_delete(&this->root);

    this->size = 0;
}

size_t hash_tree_size(struct hash_tree_t *this) {
    return this->size;
}

void *hash_tree_insert(struct hash_tree_t *this, void *data,  ...) {
    va_list args;
    va_start(args, data);

    char *key = va_arg(args, char*);
    if(!key) {
        va_end(args);
        return 0;
    }

    struct hash_tree_node_t *entry = 0;
    struct hash_map_t *tree = &this->root;
    for(;;) {
        entry = hash_map_get(tree, key);
        if(!entry) {
            entry = malloc(sizeof(struct hash_tree_node_t));
            if(!entry) {
                printf("malloc failed!\n");
                exit(-1);
            }
            entry->data = 0;
            entry->tree = 0;
            hash_map_insert(tree, key, entry);
        }
        
        key = va_arg(args, char*);

        if(!key) {
            if(entry->data) {
                va_end(args);
                return entry->data;
            } 
            entry->data = data;
            this->size++;
            va_end(args);
            return 0;
        } else {
            if(!entry->tree) {
                entry->tree = malloc(sizeof(struct hash_map_t));
                if(!entry->tree) {
                    printf("malloc failed!\n");
                    exit(-1);
                }
                hash_map_init(entry->tree, this->map_len);
            }

            tree = entry->tree;
            continue; 
        }
    }
}

void *hash_tree_get(struct hash_tree_t *this, ...) {
    va_list args;
    va_start(args, this);

    char *key = va_arg(args, char*);
    if(!key) {
        va_end(args);
        return 0;
    }

    struct hash_tree_node_t *entry = 0;
    struct hash_map_t *tree = &this->root;
    for(;;) {
        entry = hash_map_get(tree, key);
        if(!entry) {
            va_end(args);
            return 0;
        }

        key = va_arg(args, char*);

        if(!key) {
            va_end(args);
            return entry->data;
        } else {
            if(!entry->tree) {
                va_end(args);
                return 0;
            }
            tree = entry->tree;
            continue;
        }
    }
}

struct hash_tree_delete_ref_t {
    struct hash_map_t *parent;
    int entry_count;
};

void hash_tree_for_each_delete_entries(char *key, struct hash_tree_node_t *data, struct hash_tree_delete_ref_t *ref) {
    if(data->data) {
        (ref->entry_count)++;
        return;
    }

    if(data->tree) {
        struct hash_tree_delete_ref_t h_ref = {
            .parent = data->tree,
            .entry_count = 0
        };

        hash_map_for_each(data->tree, (hash_map_for_each_f)&hash_tree_for_each_delete_entries, &h_ref);
        if(!(h_ref.entry_count)) {
            hash_map_remove(h_ref.parent, key);
            hash_map_delete(data->tree);
            free(data->tree);
            free(data);
        } else {
            (ref->entry_count)++;
        }
    }
}

void *hash_tree_remove(struct hash_tree_t *this, ...) {
    va_list args;
    va_start(args, this);

    char *key = va_arg(args, char*);
    if(!key) {
        va_end(args);
        return 0;
    }

    struct hash_tree_node_t* entry = 0;
    struct hash_map_t *tree = &this->root;
    for(;;) {
        entry = hash_map_get(tree, key);
        if(!entry) {
            va_end(args);
            return 0;
        }

        key = va_arg(args, char*);

        if(!key) {
            //delete substrees
            //check if subtrees have entries
            struct hash_tree_delete_ref_t ref = {
                .parent = tree,
                .entry_count = 0
            };
            //for each
            hash_map_for_each(tree, (hash_map_for_each_f)&hash_tree_for_each_delete_entries, &ref);
            
            void *res = entry->data;
            entry->data = 0;

            if(!ref.entry_count) {
                hash_map_remove(tree, key);
                if(entry->tree) {
                    hash_map_delete(entry->tree);
                    free(entry->tree);
                }
                free(entry);
            }

            this->size--;
            return res;

        } else {
            if(!entry->tree) {
                va_end(args);
                return 0;
            }
            tree = entry->tree;
            continue;
        }
    }
}

struct hash_tree_for_each_call_ref_t {
    void (*fun)(void *data);
};

void hash_tree_for_each_call(char *key, struct hash_tree_node_t *data, struct hash_tree_for_each_call_ref_t *ref) {
    if(data->data) {
        ref->fun(data->data); 
    }

    if(data->tree) {
        hash_map_for_each(data->tree, (hash_map_for_each_f)&hash_tree_for_each_call, ref);
    }
}

void hash_tree_call(struct hash_tree_t *this, void (*fun)(void *data)) {
    struct hash_tree_for_each_call_ref_t ref = {
        .fun = fun
    };
    hash_map_for_each(&this->root, (hash_map_for_each_f)&hash_tree_for_each_call, &ref);
}

struct hash_tree_for_each_ref_t {
    void *ref;
    char **keys;
    int depth;
    void (*fun)(void *data, void *ref, char **keys);
};

void hash_tree_for_each_for_each(char *key, struct hash_tree_node_t *data, struct hash_tree_for_each_ref_t *ref) {
    ref->keys[ref->depth] = key;

    if(data->data) {
        ref->fun(data->data, ref->ref, ref->keys);
    }

    if(data->tree) {
        ref->depth++;
        hash_map_for_each(data->tree, (hash_map_for_each_f)&hash_tree_for_each_for_each, ref);
        ref->depth--;
    }
    ref->keys[ref->depth] = 0;
}

void hash_tree_for_each(struct hash_tree_t *this, void (*fun)(void *data, void *ref, char **keys), void *ref) { 
    char *keys[32] = {0}; 

    struct hash_tree_for_each_ref_t h_ref = {
        .keys = keys,
        .ref = ref,
        .depth = 0,
        .fun = fun
    };

    hash_map_for_each(&this->root, (hash_map_for_each_f)&hash_tree_for_each_for_each, &h_ref);
}


