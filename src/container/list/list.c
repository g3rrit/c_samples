#include "list.h"

#include <stdlib.h>

//---------- LIST ---------- 

void list_init(struct list_t *this)
{
    this->size = 0;
    this->head = 0;
    this->tail = 0;
}

void list_delete(struct list_t *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;
    struct list_node *free_entry;

    while(entry->next)
    {
        free_entry = entry;
        entry = entry->next;
        free(free_entry);
    }
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void list_delete_all(struct list_t *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;
    struct list_node *free_entry;

    while(entry->next)
    {
        if(!entry->data)
            free(entry->data);

        free_entry = entry;
        entry = entry->next;
        free(free_entry);
    }
    if(!entry->data)
        free(entry->data);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

struct list_node *list_push_front(struct list_t *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node));

    entry->data = data;
    entry->next = this->head;
    entry->prev = 0;

    if(this->head)
        this->head->prev = entry;
    else
        this->head = entry;

    if(!this->tail)
        this->tail = entry;
    this->size++;

    this->head = entry; 
    return entry;
}

struct list_node *list_push_back(struct list_t *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node)); 

    entry->data = data;
    entry->next = 0;
    entry->prev = this->tail;

    if(this->tail)
        this->tail->next = entry;
    else
        this->tail = entry;

    if(!this->head)
        this->head = entry;
    this->size++;

    this->tail = entry;
    return entry;
}

void *list_pop_front(struct list_t *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->head;
    void *data = entry->data;

    if(entry->next)
    {
        this->head = entry->next;
        this->head->prev = 0;
    }
    else
    {
        this->head = 0;
        this->tail = 0;
    }
    this->size--;

    free(entry);
    return data;
}

void *list_pop_back(struct list_t *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->tail;
    void *data = entry->data;

    if(entry->prev)
    {
        this->tail = entry->prev;
        this->tail->next = 0;
    }
    else
    {
        this->head = 0;
        this->tail = 0;
    }
    this->size--;

    free(entry);
    return data;
}

void *list_at(struct list_t *this, int x)
{
    if(!this->head)
        return 0;

    struct list_node *entry;
    //walk forward if x < size/2
    if(x < this->size/2)
    {
        entry = this->head;
        while(entry->next && --x >= 0)
        {
            entry = entry->next;     
        }
    }
    else
    {
        x = this->size - x;
        x--;

        entry = this->tail;
        while(entry->prev && --x >= 0)
        {
            entry = entry->prev;
        }
    }

    return entry->data;
}

void *list_remove_at(struct list_t *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct list_node **entry;
    //walk forward if x < size/2
    if(x < this->size/2)
    {
        entry = &this->head;
        while((*entry)->next && --x >= 0)
        {
            entry = &(*entry)->next;
        }
        if((*entry)->next )
        {
            (*entry)->next->prev = (*entry)->prev;
        }
        else
        {
            this->tail = (*entry)->prev;
        }
        
        data = (*entry)->data;

        struct list_node *free_entry = *entry;

        *entry = (*entry)->next;

        free(free_entry);
    }
    else
    {
        x = this->size - x;
        x--;

        entry = &this->tail;
        while((*entry)->prev && --x >= 0)
        {
            entry = &(*entry)->prev;
        }
        if((*entry)->prev)
        {
            (*entry)->prev->next = (*entry)->next;
        }
        else
        {
            this->head = (*entry)->next;
        }

        data = (*entry)->data;

        struct list_node *free_entry = *entry;

        *entry = (*entry)->prev;

        free(free_entry);
    }
    this->size--;

    if(!this->size)
    {
        this->head = 0;
        this->tail = 0;
    }
    
    return data;
}

void *list_remove_node(struct list_t *this, struct list_node *node)
{
    if(!node->prev)
    {
        this->head = node->next;
        if(node->next)
            node->next->prev = 0;
    }

    if(!node->next)
    {
        this->tail = node->prev;
        if(node->prev)
            node->prev->next = 0;
    }

    if(node->next && node->prev)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    this->size--;

    void *resdata = node->data;

    free(node);

    return resdata;
}

void list_call(struct list_t *this, void (*fun)(void *data)) {
    if(!(this->head))
        return;

    struct list_node *entry = this->head;
    struct list_node *next;

    while(entry) {
        next = entry->next;
        fun(entry->data);
        entry = next;
    }
}

void list_for_each(struct list_t *this, void (*fun)(void *data, void *ref), void *ref)
{
    if(!(this->head))
        return;

    struct list_node *entry = (this->head);

    void *retval = 0;

    /*
    struct list_info info;
    info.this = this;
    info.entry = entry;
    info.pos = 0;
    info.size = this->size;
    */

    /*
    while(!(retval = fun((*entry)->data, ref, &info)) && (*entry)->next)
    {
        entry = &(*entry)->next;

        info.entry = *entry;
        info.pos++;
    }
    */

    //you should be able to remove nodes while inside for_each
    //so the walking is done bevor executing the for_each_function

    int run_l = 1;
    void *e_data;
    do
    {
        e_data = entry->data;
        info.entry = entry;
        info.pos++;

        if(entry->next)
            entry = entry->next;
        else
            run_l = 0;

        fun(e_data, ref);
    }
    while(run_l);
}
