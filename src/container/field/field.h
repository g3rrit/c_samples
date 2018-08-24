#ifndef FIELD_H
#define FIELD_H

// ---------- FIELD ----------  

struct field_node
{
    void *data;
    int *pos;
};

struct field_t
{
    struct field_node *data; 
    int size;
    int allocated_size;
};

struct field_info
{
    struct list *this;
    int pos;
    int size;
};

void field_init(struct field_t *this, int size);

int field_resize(struct field_t *this, int size);

void field_push_back(struct field_t *this, void *data, int *pos);
void field_push_front(struct field_t *this, void *data, int *pos);

void *field_pop_front(struct field_t *this);
void *field_pop_back(struct field_t *this);

void *field_at(struct field_t *this, int x);

void field_set(struct field_t *this, int *pos, void *data);

void field_delete(struct field_t *this);
void field_delete_all(struct field_t *this);

void *field_remove_at(struct field_t *this, int x);

void *field_for_each(struct field_t *this, void *(*fun)(void *data, void *ref, struct field_info *info), void *ref);

#endif


