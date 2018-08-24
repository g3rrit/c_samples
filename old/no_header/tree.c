#ifndef TREE_H
#define TREE_H

struct tree_node
{
    void *val;
    int pos;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree
{
    struct tree_node *root;
    int size;
};

void tree_init(struct tree *this);

int tree_add(struct tree *this, void *val, int pos);

int tree_remove(struct tree *this, int pos);

int tree_delete(struct tree *this);

#endif

#ifndef TREE_C

struct tree_node *rot_left(

void tree_init(struct tree *this)
{
    this->root = 0;
    this->size = 0;
}

int tree_add(struct tree *this, void *val, int pos)
{
    struct tree_node **entry = &this->root;

    
}

int tree_remove(struct tree *this, int pos)
{
}

int tree_delete(struct tree *this)
{
}

#endif
