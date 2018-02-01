#define EVENT_C
#include "event.c"
#undef EVENT_C

#include "stdio.h"
#include "snow.h"

void *print_stuff(void *ref, void *data)
{
    if(!(strcmp(ref, "ref3")))
        return data;
    else
        return 0;
}

describe(event,
{
    it("add_event",
    {
        struct event mevent;
        event_init(&mevent);

        char *ckey = malloc(5);
        strcpy(ckey,"1key");
        char *cid = malloc(5);
        strcpy(cid, "1id1");
        char *cref1 = "ref1";
        char *cref2 = "ref2";
        char *cref3 = "ref3";
        char *cref4 = "ref4";
        for(int i = 0; i < 9; i++)
        {
            event_add(&mevent, ckey, cid, &print_stuff, cref1);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref2);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref3);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref4);

            ckey[0] += 1;
            cid[0] += 1;
            cid[3] = '1';
        }

        asserteq(mevent.event_map.size, 9);

        strcpy(ckey, "1key");
        int rdata = 0;

        for(int i = 0; i < 9; i++)
        {
            asserteq(event_dispatch(&mevent, ckey, rdata), i);

            rdata++;
            ckey[0] += 1;
        }
    });

    it("remove_event",
    {
        struct event mevent;
        event_init(&mevent);

        char *ckey = malloc(5);
        strcpy(ckey,"1key");
        char *cid = malloc(5);
        strcpy(cid, "1id1");
        char *cref1 = "ref1";
        char *cref2 = "ref2";
        char *cref3 = "ref3";
        char *cref4 = "ref4";
        for(int i = 0; i < 9; i++)
        {
            event_add(&mevent, ckey, cid, &print_stuff, cref1);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref2);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref3);
            cid[3]++;
            event_add(&mevent, ckey, cid, &print_stuff, cref4);

            ckey[0] += 1;
            cid[0] += 1;
            cid[3] = '1';
        }

        asserteq(mevent.event_map.size, 9);

        strcpy(cid, "1id1");
        for(int i = 0; i < 7; i++)
        {
            event_remove(&mevent, cid);
            cid[3]++;
            event_remove(&mevent, cid);
            cid[3]++;
            event_remove(&mevent, cid);
            cid[3]++;
            event_remove(&mevent, cid);
            cid[0]++;
            printf("here\n");
            asserteq(mevent.event_map.size, 9-i);
        }

        event_clear(&mevent);

        asserteq(&mevent.event_map.size, 0);
    });
});

snow_main(
{
    test_event();
});

