//hdr
#ifndef CONTAINER_H
#define CONTAINER_H

struct container
{
    void (*delete)(void *this);
    void (*delete_all)(void *this);

    void *(*push_front)(void *this, void *data);
    void *(*push_back)(void *this, void *data);

    void *(*pop_front)(void *this);
    void *(*pop_back)(void *this);

    void *(*at)(void *this, int x);
    void *(*remove_at)(void *this, int x);
};

void container_delete(struct container *this);
void container_delete_all(struct container *this);

void container_push_front(struct container *this, void *data);
void container_push_back(struct container *this, void *data);

void *container_pop_front(struct container *this);
void *container_pop_back(struct container *this);

void *container_at(struct container *this, int x);
void *container_remove_at(struct container *this, int x);

#endif

//src
#ifndef CONTAINER_C

void container_delete(struct container *this)
{
    this->delete(this);
}

void container_delete_all(struct container *this)
{
    this->delete_all(this);
}

void container_push_front(struct container *this, void *data)
{
    this->push_front(this, data);
}

void container_push_back(struct container *this, void *data)
{
    this->push_back(this, data);
}

void *container_pop_front(struct container *this)
{
    return this->pop_front(this);
}

void *container_pop_back(struct container *this)
{
    return this->pop_back(this);
}

void *container_at(struct container *this, int x)
{
    return this->at(this, x);
}

void *container_remove_at(struct container *this, int x)
{
    return this->remove_at(this, x);
}

#endif
