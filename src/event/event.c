//hdr
#ifndef EVENT_H
#define EVENT_H

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

struct event_data
{
    void *(*fun)(void *ref, void *data);
    void *ref;
};

struct event
{
    struct map event_map;
};

void event_init(struct event *this);

void event_add(struct event *this, char *key, char *id, void *(*fun)(void *ref, void *data), void *ref);

void event_remove(struct event *this, char *id);

void *event_dispatch(struct event *this, char *key, void *data);

void event_clear(struct event *this);

#endif

//src
#ifndef EVENT_C

void *event_search_remove(struct map *_map, char *id, struct map *map_data);
void *event_for_each_dispatch(struct event_data *t_event_data, void *data, struct map *_map);
void *event_for_each_clear(struct map *_map, void *emp, struct map *map_data);

void event_init(struct event *this)
{
    map_init(&(this->event_map));
}

void event_add(struct event *this, char *key, char *id, void *(*fun)(void *ref, void *data), void *ref)
{
    struct map *map_entry = 0;
    if(!(map_entry = map_get(&(this->event_map), key)))
    {
        struct map *new_map = malloc(sizeof(struct map));
        map_init(new_map);
        map_push_back(&(this->event_map), key, new_map);
        map_entry = new_map;
    }

    struct event_data *new_event_data = malloc(sizeof(struct event_data));
    new_event_data->ref = ref;
    new_event_data->fun = fun; 

    map_push_back(map_entry, id, new_event_data);
}

void event_remove(struct event *this, char *id)
{
    map_for_each(&(this->event_map), &event_search_remove, id);
}

void *event_dispatch(struct event *this, char *key, void *data)
{
    struct map *t_map = map_get(&(this->event_map), key);
    if(t_map)
        return map_for_each(t_map, &event_for_each_dispatch, data);
    else
        return 0;
}

void event_clear(struct event *this)
{
    map_for_each(&(this->event_map), &event_for_each_clear, 0);
    map_delete_all(&(this->event_map));
    event_init(this);
}


//foreach functions
void *event_search_remove(struct map *_map, char *id, struct map *map_data)
{
    struct event_data *t_event_data = map_remove(_map, id);
    if(t_event_data)
        free(t_event_data);
    return 0;
}

void *event_for_each_dispatch(struct event_data *t_event_data, void *data, struct map *_map)
{
    return t_event_data->fun(t_event_data->ref, data);
}

void *event_for_each_clear(struct map *_map, void *emp, struct map *map_data)
{
    map_delete_all(_map);
    return 0;
}

#endif

