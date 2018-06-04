#ifndef TREE_H
#define TREE_H

struct tree_node
{
    void *val;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree
{
    struct tree_node *root;
    int size;
};

int tree_init(struct tree *this);

#endif

#ifndef TREE_C

#endif
