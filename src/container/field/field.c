#include "field.h"

#define test_resize() if(this->size >= this->allocated_size) field_resize(this, this->size + 10)
#define test_shrink() if(this->size <= this->allocated_size - 20) field_resize(this, this->size + 10)

#define set_zero(i) memset(&(this->data[i]), 0, sizeof(struct field_node))
#define check_zero(i) this->data[i].data
#define copy_data(dest, src) memcpy(&(this->data[dest]), &(this->data[src]), sizeof(struct field_node))

void field_init(struct field_t *this, int size)
{
    this->data = malloc(sizeof(struct field_node) * size);
    this->size = 0;
    this->allocated_size = size;

    for(int i = 0; i < size; i++)
        set_zero(i);
}

int field_resize(struct field_t *this, int size)
{
    if(this->size >= size)
        return 0;

    this->data = realloc(this->data, sizeof(struct field_node) * size);

    this->allocated_size = size;
    for(int i = this->size; i < this->allocated_size; i++)
        set_zero(i);

    if(!this->data)
        return 0;
    else
        return 1;
}

void field_push_back(struct field_t *this, void *data, int *pos)
{
    test_resize(); 

    this->data[this->size].data = data;
    this->data[this->size].pos = pos;
    *pos = this->size;

    this->size++;
}

void field_push_front(struct field_t *this, void *data, int *pos)
{
    test_resize(); 

    *(this->data[0].pos) = this->size;
    copy_data(this->size, 0);

    this->size++;

    this->data[0].data = data;
    this->data[0].pos = pos;
    *pos = 0;
}

void *field_pop_front(struct field_t *this)
{
    void *res = this->data[0].data;

    if(this->size > 1)
    {
        copy_data(0, this->size - 1);
        *(this->data[0].pos) = 0;
    }
    else
    {
        set_zero(0);
    }

    this->size--;

    test_shrink();

    return res;
}

void *field_pop_back(struct field_t *this)
{
    void *res = this->data[this->size - 1].data;

    set_zero(this->size - 1);

    this->size--;

    test_shrink();

    return res;
}

void *field_at(struct field_t *this, int x)
{
    return this->data[x].data;
}

void field_set(struct field_t *this, int *pos, void *data)
{
    if(this->allocated_size <= *pos)
       field_resize(this, *pos + 10);

    if(this->size <= *pos)
        this->size = *pos + 1;

    this->data[*pos].data = data;
    this->data[*pos].pos = pos;
}

void field_delete(struct field_t *this)
{
    if(this->data)
        free(this->data);
    this->size = 0;
    this->allocated_size = 0;
    this->data = 0;
}

void field_delete_all(struct field_t *this)
{
    for(int i = 0; i < this->size; i++)
    {
        if(check_zero(i))
            free(this->data[i].data);
    }
    field_delete(this);
}

void *field_remove_at(struct field_t *this, int x)
{
    void *res = this->data[x].data;

    this->size--;
    if(this->size > 0)
    {
        if(x == this->size - 1)
        {
            return field_pop_back(this);
        }
        else
        {
            copy_data(x, this->size);
            set_zero(this->size);
            this->data[x].pos = x;
        }
    }
    else
    {
        return field_pop_back(this);
    }

    return res;
}

void *field_for_each(struct field_t *this, void *(*fun)(void *data, void *ref, struct field_info *info), void *ref)
{
    struct field_info info;
    info.this = this;
    info.size = this->size;
    info.pos = 0;

    void *retval = 0;
    for(int i = 0; i < this->size; i++)
    {
        if((retval = fun(this->data[i].data, ref, &info)))
            return retval;
        info.pos++;
    }
    return 0;
}



