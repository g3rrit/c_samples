
#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

#include "snow.h"

describe(container, 
{    
    it("push_front", 
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_front(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,0) , i);
        }        
        asserteq(mlist.size, 10);
    });

    it("push_back", 
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_back(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,9) , i);
        }        
        asserteq(mlist.size, 10);
    });

    it("list_delete", 
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_front(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,0) , i);
        } 
        list_delete(&mlist);
        assert(!mlist.size);
        assert(!mlist.head);
        assert(!mlist.tail);
    });

});

snow_main(
{
    test_container();
});
