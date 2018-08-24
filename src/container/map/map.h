#ifndef MAP_H
#define MAP_H

//---------- MAP ---------- 

struct map_node
{
    struct map_node *next;
#ifndef S_LINKED
    struct map_node *prev;
#endif
    char *key;
    void *data;
};

struct map_t
{
    struct map_node *head;
    struct map_node *tail;
    int size;
};

struct map_info
{
    struct map_t *this;
    struct map_node *entry;
    char *key;
    int pos;
    int size;
};

void map_init(struct map_t *this);

void map_delete(struct map_t *this);
void map_delete_all(struct map_t *this);

void map_push_front(struct map_t *this, char *key, void *data);
void map_push_back(struct map_t *this, char *key, void *data);

void *map_pop_front(struct map_t *this);
void *map_pop_back(struct map_t *this);

void *map_get(struct map_t *this, char *key);
void *map_at(struct map_t *this, int x);

void *map_remove(struct map_t *this, char *key);
void *map_remove_at(struct map_t *this, int x);

void *map_for_each(struct map_t *this, void *(*fun)(void *data, void *ref, struct map_info *info), void *ref);

#endif


