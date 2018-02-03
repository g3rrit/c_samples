//hdr
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#define OBJECT_C
#include "object.c"
#undef OBJECT_C

struct object_manager
{
    struct object object_array[10];
};

void object_manager_init(struct object_manager *this);

void object_manager_fill(struct object_manager *this);

void object_manager_add_object(struct object_manager *this, struct object *_object);

void object_manager_draw(struct object_manager *this);

#endif

//src
#ifndef OBJECTMANAGER_C

void object_manager_init(struct object_manager *this)
{
}

void object_manager_fill(struct object_manager *this)
{
    for(int i = 0; i < 10; i++)
    {
        struct text_field *tempfield = malloc(sizeof(struct text_field));
        text_field_init(tempfield, "hall", 1, i + 1);


        struct object tempobject;
        object_init(&tempobject, tempfield, &text_field_draw);
        this->object_array[i] = tempobject;
    }
}

void object_manager_add_object(struct object_manager *this, struct object *_object)
{

}

void object_manager_draw(struct object_manager *this)
{
    for(int i = 0; i < 10; i++)
    {
        object_draw(this->object_array + i);
    }
}

#endif
