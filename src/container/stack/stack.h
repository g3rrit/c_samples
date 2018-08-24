#ifndef STACK_H
#define STACK_H

struct stack_node
{
    void *data;
    struct stack_node *prev;
};

struct stack_t
{
    struct stack_node *top;
    int size;
};

void stack_init(struct stack_t *this);

void stack_push(struct stack_t *this, void *data);

void *stack_pop(struct stack_t *this);

void *stack_top(struct stack_t *this);

void stack_delete(struct stack_t *this);

void stack_delete_all(struct stack_t *this);

#endif


