//hdr
#ifndef CONTAINER_H
#define CONTAINER_H

//---------- LIST ---------- 

struct list_node
{
    struct list_node *next;
#ifndef S_LINKED
    struct list_node *prev;
#endif
    void *data;
};

struct list
{
    struct list_node *head;
    struct list_node *tail;
    int size;
};

struct list_info
{
    struct list *this;
    struct list_node *entry;
    int pos;
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

//if fun returns something other than 0 stop
//returns 0 if all objects where iterated
void *list_for_each(struct list *this, void *(*fun)(void *data, void *ref, struct list_info *info), void *ref);

//------------------------- 

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

struct map
{
    struct map_node *head;
    struct map_node *tail;
    int size;
};

struct map_info
{
    struct map *this;
    struct map_node *entry;
    char *key;
    int pos;
    int size;
};

void map_init(struct map *this);

void map_delete(struct map *this);
void map_delete_all(struct map *this);

void map_push_front(struct map *this, char *key, void *data);
void map_push_back(struct map *this, char *key, void *data);

void *map_pop_front(struct map *this);
void *map_pop_back(struct map *this);

void *map_get(struct map *this, char *key);
void *map_at(struct map *this, int x);

void *map_remove(struct map *this, char *key);
void *map_remove_at(struct map *this, int x);

void *map_for_each(struct map *this, void *(*fun)(void *data, void *ref, struct map_info *info), void *ref);

//---------- VECTOR ---------- 

struct vector
{
    void **data;
    int size;
    int allocated_size;
    int size_of_element;
};

struct vector_info
{
    struct vector *this;
    int pos;
    int size;
};

void vector_init(struct vector *this, int size_of_element, int size);

int vector_resize(struct vector *this, int size);

void vector_push_front(struct vector *this, void *data);
void vector_push_back(struct vector *this, void *data);

void *vector_pop_front(struct vector *this);
void *vector_pop_back(struct vector *this);

void *vector_at(struct vector *this, int x);

int vector_set(struct vector *this, int x, void *data);

void vector_delete(struct vector *this);

void vector_delete_all(struct vector *this);

void *vector_remove_at(struct vector *this, int x);

void *vector_remove_at_unordered(struct vector *this, int x);

void *vector_for_each(struct vector *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref);

//------------------------- 

#endif

//src
#ifndef CONTAINER_C

#include <stdlib.h>
#include <string.h>

//---------- LIST ---------- 

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

void list_delete_all(struct list *this)
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

void list_push_front(struct list *this, void *data)
{
    struct list_node *entry = malloc(sizeof(struct list_node));

    entry->data = data;
    entry->next = this->head;
#ifndef S_LINKED
    entry->prev = 0;
#endif

#ifndef S_LINKED
    if(this->head)
        this->head->prev = entry;
#endif

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
#ifndef S_LINKED
    entry->prev = this->tail;
#endif

    if(this->tail)
        this->tail->next = entry;

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
#ifndef S_LINKED
        this->head->prev = 0;
#endif
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
#ifndef S_LINKED
        this->tail->next = 0;
#endif
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
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
        entry = this->head;
        while(entry->next && --x >= 0)
        {
            entry = entry->next;     
        }
#ifndef S_LINKED
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
#endif

    return entry->data;
}

void *list_remove_at(struct list *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct list_node **entry;
    //walk forward if x < size/2
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
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
#ifndef S_LINKED
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
#endif
    this->size--;

    if(!this->size)
    {
        this->head = 0;
        this->tail = 0;
    }
    
    return data;
}

void *list_for_each(struct list *this, void *(*fun)(void *data, void *ref, struct list_info *info), void *ref)
{
    if(!(this->head))
        return 0;

    struct list_node **entry = &(this->head);

    void *retval = 0;

    struct list_info info;
    info.this = this;
    info.entry = *entry;
    info.pos = 0;
    info.size = this->size;

    while(!(retval = fun((*entry)->data, ref, &info)) && (*entry)->next)
    {
        entry = &(*entry)->next;

        info.entry = *entry;
        info.pos++;
    }
    return retval;
}

//------------------------- 

//---------- MAP ---------- 

void map_init(struct map *this)
{
    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void map_delete(struct map *this)
{
    if(!this->head)
        return;

    struct map_node *entry = this->head;
    struct map_node *free_entry;

    while(entry->next)
    {
        free_entry = entry;
        entry = entry->next;
        free(free_entry->key);
        free(free_entry);
    }
    free(entry->key);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

void map_delete_all(struct map *this)
{
    if(!this->head)
        return;

    struct map_node *entry = this->head;
    struct map_node *free_entry;

    while(entry->next)
    {
        if(!entry->data)
            free(entry->data);

        free_entry = entry;
        entry = entry->next;
        free(free_entry->key);
        free(free_entry);
    }
    if(!entry->data)
        free(entry->data);
    free(entry->key);
    free(entry);

    this->head = 0;
    this->tail = 0;
    this->size = 0;
}

//note: key get saved in map_node without 0 terminator

void map_push_front(struct map *this, char *key, void *data)
{
    struct map_node *entry = malloc(sizeof(struct map_node));

    entry->data = data;
    entry->key = malloc(sizeof(char) * strlen(key));
    memcpy(entry->key, key, strlen(key));
    entry->next = this->head;
#ifndef S_LINKED
    entry->prev = 0;
#endif

#ifndef S_LINKED
    if(this->head)
        this->head->prev = entry;
#endif

    if(!this->tail)
        this->tail = entry;
    this->size++;

    this->head = entry; 
}

void map_push_back(struct map *this, char *key, void *data)
{
    struct map_node *entry = malloc(sizeof(struct map_node)); 

    entry->data = data;
    entry->key = malloc(sizeof(char) * strlen(key));
    memcpy(entry->key, key, strlen(key));
    entry->next = 0;
#ifndef S_LINKED
    entry->prev = this->tail;
#endif

    if(this->tail)
        this->tail->next = entry;

    if(!this->head)
        this->head = entry;
    this->size++;

    this->tail = entry;
}

void *map_pop_front(struct map *this)
{
    if(!this->head)
        return 0;

    struct map_node *entry = this->head;
    void *data = entry->data;

    if(entry->next)
    {
        this->head = entry->next;
#ifndef S_LINKED
        this->head->prev = 0;
#endif
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

void *map_pop_back(struct map *this)
{
    if(!this->head)
        return 0;

    struct map_node *entry = this->tail;
    void *data = entry->data;

    if(entry->prev)
    {
        this->tail = entry->prev;
#ifndef S_LINKED
        this->tail->next = 0;
#endif
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

void *map_get(struct map *this, char *key)
{
    if(!this->head)
        return 0;

    struct map_node *entry;
    entry = this->head;
    int rescomp = 1;
    while((rescomp = strncmp(entry->key, key, strlen(key))) && entry->next)
    {
        entry = entry->next;     
    }

    if(!rescomp)
        return entry->data;
    else
        return 0;
}

void *map_at(struct map *this, int x)
{
    if(!this->head)
        return 0;

    struct map_node *entry;
    //walk forward if x < size/2
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
        entry = this->head;
        while(entry->next && --x >= 0)
        {
            entry = entry->next;     
        }
#ifndef S_LINKED
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
#endif

    return entry->data;

}

void *map_remove(struct map *this, char *key)
{
    if(!this->head)
        return 0;

    void *data;

    struct map_node **entry;
    entry = &this->head;
    int rescmp = 1;
    while((rescmp = strncmp((*entry)->key, key, strlen(key))) && (*entry)->next)
    {
        entry = &(*entry)->next;
    }
    if(!rescmp)
    {
        if((*entry)->next )
        {
            (*entry)->next->prev = (*entry)->prev;
        }
        else
        {
            this->tail = (*entry)->prev;
        }

        data = (*entry)->data;

        struct map_node *free_entry = *entry;

        *entry = (*entry)->next;

        free(free_entry);
        this->size--;

        if(!this->size)
        {
            this->head = 0;
            this->tail = 0;
        }

        return data;
    }
    else
    {
        return 0;
    }

}

void *map_remove_at(struct map *this, int x)
{
    if(!this->head)
        return 0;

    void *data;

    struct map_node **entry;
    //walk forward if x < size/2
#ifndef S_LINKED
    if(x < this->size/2)
    {
#endif
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

        struct map_node *free_entry = *entry;

        *entry = (*entry)->next;

        free(free_entry);
#ifndef S_LINKED
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

        struct map_node *free_entry = *entry;

        *entry = (*entry)->prev;

        free(free_entry);
    }
#endif
    this->size--;

    if(!this->size)
    {
        this->head = 0;
        this->tail = 0;
    }
    
    return data;

}

void *map_for_each(struct map *this, void *(*fun)(void *data, void *ref, struct map_info *info), void *ref)
{
    if(!(this->head))
        return 0;

    struct map_node **entry = &(this->head);

    void *retval = 0;

    struct map_info info;
    info.this = this;
    info.entry = *entry;
    info.pos = 0;
    info.size = this->size;
    info.key = (*entry)->key;

    while(!(retval = fun((*entry)->data, ref, &info)) && (*entry)->next)
    {
        entry = &(*entry)->next;

        info.entry = *entry;
        info.pos++;
        info.key = (*entry)->key;
    }
    return retval;
}

//------------------------- 

//---------- MAP ---------- 

#define test_resize() if(this->size >= this->allocated_size) vector_resize(this, this->size + 10)
#define test_shrink() if(this->size <= this->allocated_size - 20) vector_resize(this, this->size + 10)

void vector_init(struct vector *this, int size_of_element, int size)
{
    this->size = 0;
    this->size_of_element = size_of_element;
    this->data = malloc(sizeof(void*) * size);
    this->allocated_size = size;
}

int vector_resize(struct vector *this, int size)
{
    if(this->size >= size)
        return 0;

    this->allocated_size = size;
    //realloc data 
    this->data = realloc(this->data, sizeof(void*) * size);

    if(!this->data)
        return 0;
    else
        return 1;
}

void vector_push_front(struct vector *this, void *data)
{
    test_resize();

    for(int i = this->size; i >= 0; i--)
        this->data[i] = this->data[i-1];          

    this->size++;

    this->data[0] = data;
}

void vector_push_back(struct vector *this, void *data)
{
    test_resize();

    this->data[this->size] = data;
    this->size++;
}

void *vector_pop_front(struct vector *this)
{
    void *res = this->data[0];

    for(int i = 0; i < this->size - 1; i++)
        this->data[i] = this->data[i+1];

    this->data[this->size-1] = 0;

    this->size--;

    test_shrink();

    return res;
}

void *vector_pop_back(struct vector *this)
{
    void *data = this->data[this->size-1];
    this->data[this->size - 1] = 0;
    this->size--;

    test_shrink();

    return data;
}

void *vector_at(struct vector *this, int x)
{
    return this->data[x];
}

int vector_set(struct vector *this, int x, void *data)
{
    if(this->allocated_size <= x)
       vector_resize(this, x + 10);

    if(this->size <= x)
        this->size = x+1;

    this->data[x] = data;
    return 1;
}

void vector_delete(struct vector *this)
{
    free(this->data);
    this->size = 0;
    this->allocated_size = 0;
    this->data = 0;
    this->size_of_element = 0;
}

void vector_delete_all(struct vector *this)
{
    for(int i = 0; i < this->size; i++)
    {
        free(this->data[i]);
    }
    vector_delete(this);
}

//unordered
void *vector_remove_at(struct vector *this, int x)
{
    void *res = this->data[x]; 

    for(; x < this->size - 1; x++)
        this->data[x] = this->data[x+1];

    this->data[this->size - 1] = 0;

    this->size--;

    test_shrink();

    return res;
}

//ordered
void *vector_remove_at_unordered(struct vector *this, int x)
{
    if(x >= this->size)
        return 0;

    void *res = this->data[x];
    this->data[x] = this->data[this->size - 1];
    this->data[this->size - 1] = 0;
    this->size--;

    test_shrink();

    return res;
}

void *vector_for_each(struct vector *this, void *(fun)(void *data, void *ref, struct vector_info *info), void *ref)
{
    
    struct vector_info info;
    info.this = this;
    info.pos = 0;
    info.size = this->size;

    void *retval = 0;
    for(int i = 0; i < this->size; i++)
    {
        if((retval = fun(this->data[i], ref, &info)))
            return retval;
        info.pos++;
    }
    return 0;
}

//------------------------- 
#endif
