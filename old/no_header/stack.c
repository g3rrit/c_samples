#ifndef STACK_H
#define STACK_H

struct stack_node
{
    void *data;
    struct stack_node *prev;
};

struct stack
{
    struct stack_node *top;
    int size;
};

void stack_init(struct stack *this);

void stack_push(struct stack *this, void *data);

void *stack_pop(struct stack *this);

void *stack_top(struct stack *this);

void stack_delete(struct stack *this);

void stack_delete_all(struct stack *this);

#endif

#ifndef STACK_C

void stack_init(struct stack *this)
{
    this->size = 0;
    this->top = 0;
}

void stack_push(struct stack *this, void *data)
{
    struct stack_node *entry = malloc(sizeof(struct stack_node));
    entry->data = data;
    entry->prev = this->top;

    this->top = entry;

    this->size++;
}

void *stack_pop(struct stack *this)
{
    void *res = 0;

    if(this->top)
    {
        struct stack_node *top = this->top;
        res = top->data;
        this->top = top->prev;
        free(top);
    }

    this->size--;

    return res;
}

void *stack_top(struct stack *this)
{
    void *res = 0;

    if(this->top)
        res = this->top->data;

    return res;
}

void stack_delete(struct stack *this)
{
    struct stack_node *entry = this->top;
    struct stack_node *fentry = entry;

    while(entry->prev)
    {
        entry = entry->prev; 
        free(fentry);
        fentry = entry;
    }
    free(fentry);

    this->size = 0;
}

void stack_delete_all(struct stack *this)
{
    struct stack_node *entry = this->top;
    struct stack_node *fentry = entry;

    while(entry->prev)
    {
        entry = entry->prev; 
        free(fentry);
        free(fentry->data);
        fentry = entry;
    }
    free(fentry);
    free(fentry->data);

    this->size = 0;
}

#endif
