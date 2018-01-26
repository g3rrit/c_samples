//hdr
#ifndef CONTAINER_H
#define CONTAINER_H

struct list_node
{
    void *data;
    struct list_node *next;
    struct list_node *prev;
};

struct list
{
    struct list_node *head;
    struct list_node *tail;
    int size;
};

void list_init(struct list *this);

void list_delete(struct list *this);
void list_delete_all(struct list *this);

void list_push_front(struct list *this, void *data);
void list_push_back(struct list *this, void *data);

void *list_pop_front(struct list *this);
void *list_pop_back(struct list *this);

void *list_at(struct list *this, int x);
void *list_remove_at(struct list *this, int x);

#endif

//src
#ifndef CONTAINER_C

#include <stdlib.h>

void list_init(struct list *this)
{
    this->size = 0;
    this->head = 0;
    this->tail = 0;
}

void list_delete(struct list *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;

    while(entry->next)
    {
        entry = entry->next;
        free(entry->prev);
    }
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void list_delete_all(struct list *this)
{
    if(!this->head)
        return;

    struct list_node *entry = this->head;

    while(entry->next)
    {
        if(!entry->data)
            free(entry->data);

        entry = entry->next;
        free(entry->prev);
    }
    if(!entry->data)
        free(entry->data);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void list_push_front(struct list *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node));

    entry->data = data;
    entry->next = this->head;
    entry->prev = 0;

    if(!this->tail)
        this->tail = entry;
    this->size++;

    this->head = entry; 
}

void list_push_back(struct list *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node)); 

    entry->data = data;
    entry->next = 0;
    entry->prev = this->tail;

    if(!this->head)
        this->head = entry;
    this->size++;

    this->tail = entry;
}

void *list_pop_front(struct list *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->head;
    void *data = entry->data;

    if(entry->next)
    {
        this->head = entry->next;
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

void *list_pop_back(struct list *this)
{
    if(!this->head)
        return 0;

    struct list_node *entry = this->tail;
    void *data = entry->data;

    if(entry->prev)
    {
        this->tail = entry->prev;
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

void *list_at(struct list *this, int x)
{
    if(!this->head)
        return 0;

    struct list_node *entry;
    //walk forward if x < size/2
    if(x < this->size/2)
    {
        entry = this->head;
        while(entry->next && (x--) >= 0)
        {
            entry = entry->next;     
        }
    }
    else
    {
        entry = this->tail;
        while(entry->prev && (x--) >= 0)
        {
            entry = entry->prev;
        }
    }

    return entry->data;
}

void *list_remove_at(struct list *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct list_node **entry;
    //walk forward if x < size/2
    if(x < this->size/2)
    {
        entry = &this->head;
        while((*entry)->next && (x--) >= 0)
        {
            entry = &(*entry)->next;
        }
        if((*entry)->next)
        {
            (*entry)->next->prev = (*entry)->prev;
        }
        data = (*entry)->data;
        *entry = (*entry)->next;
        free(entry);
    }
    else
    {
        entry = &this->tail;
        while((*entry)->prev && (x--) >= 0)
        {
            entry = &(*entry)->prev;
        }
        if((*entry)->prev)
        {
            (*entry)->prev->next = (*entry)->next;
        }
        data = (*entry)->data;
        *entry = (*entry)->prev;
        free(entry);
    }
    this->size--;

    if(!this->size)
    {
        this->head = 0;
        this->tail = 0;
    }
    
    return data;
}

#endif

















