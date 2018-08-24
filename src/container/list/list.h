//hdr
#ifndef LIST_H
#define LIST_H

struct list_node
{
    struct list_node *next;
    struct list_node *prev;
    void *data;
};

struct list_t
{
    struct list_node *head;
    struct list_node *tail;
    int size;
};

struct list_info
{
    struct list_t *this;
    struct list_node *entry;
    int pos;
    int size;
};

void list_init(struct list_t *this);

void list_delete(struct list_t *this);
void list_delete_all(struct list_t *this);

struct list_node *list_push_front(struct list_t *this, void *data);
struct list_node *list_push_back(struct list_t *this, void *data);

void *list_pop_front(struct list_t *this);
void *list_pop_back(struct list_t *this);

void *list_at(struct list_t *this, int x);
void *list_remove_at(struct list_t *this, int x);
void *list_remove_node(struct list_t *this, struct list_node *node);

//if fun returns something other than 0 stop
//returns 0 if all objects where iterated
void *list_for_each(struct list_t *this, void *(*fun)(void *data, void *ref, struct list_info *info), void *ref);

//------------------------- 

#endif


