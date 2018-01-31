#define EVENT_C
#include "event.c"
#undef EVENT_C

#include "stdio.h"
#include "snow.h"

void *print_stuff(void *ref, void *data)
{
    printf("printing ref: %s and data: %s\n", ref, data);
}

describe(event,
{
    it("add_event",
    {
        struct event mevent;
        event_init(&mevent);
        event_add(&mevent, "key1", "1id1", &print_stuff, "1id1ref");
        event_add(&mevent, "key1", "1id2", &print_stuff, "1id2ref");
        event_add(&mevent, "key2", "2id1", &print_stuff, "2id1ref");
        event_add(&mevent, "key2", "2id2", &print_stuff, "2id2ref");
        event_add(&mevent, "key3", "3id1", &print_stuff, "3id1ref");

        event_dispatch(&mevent, "key1", "data1");
        event_dispatch(&mevent, "key2", "data2");
        event_dispatch(&mevent, "key3", "data3");

    });
});

snow_main(
{
    test_event();
});

