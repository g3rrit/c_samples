
#define CONTAINER_C
#define S_LINKED
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

    it("pop_back",
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_back(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,9) , i);
        }
        for(int i = 9; i >= 0; i--)
        {
            asserteq(list_pop_back(&mlist), i);
        }
        assert(!mlist.size);
        assert(!mlist.head);
        assert(!mlist.tail);
    });

    it("pop_front",
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_back(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,9) , i);
        }
        for(int i = 0; i < 10; i++)
        {
            asserteq(list_pop_front(&mlist), i);
        }
        assert(!mlist.size);
        assert(!mlist.head);
        assert(!mlist.tail);
    });

    it("at",
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_back(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,9) , i);
        }
        for(int i = 0; i < 10; i++)
        {
            asserteq(list_at(&mlist, i), i);
        }
        asserteq(mlist.size, 10);
        assert(mlist.head);
        assert(mlist.tail);
    });

    it("remove_at",
    {
        struct list mlist;
        list_init(&mlist);
        for(int i = 0; i < 10; i++)
        {
            list_push_back(&mlist, i);
            asserteq(mlist.size, i+1);
            asserteq(list_at(&mlist,9) , i);
        }
        asserteq(list_remove_at(&mlist, 0), 0);
        asserteq(list_remove_at(&mlist, 8), 9);
        asserteq(list_remove_at(&mlist, 3), 4);
        asserteq(list_remove_at(&mlist, 5), 7);
        asserteq(list_remove_at(&mlist, 4), 6);
        asserteq(list_remove_at(&mlist, 4), 8);
        asserteq(list_remove_at(&mlist, 1), 2);
        asserteq(list_remove_at(&mlist, 0), 1);
        asserteq(list_remove_at(&mlist, 1), 5);
        asserteq(list_remove_at(&mlist, 0), 3);

        asserteq(mlist.size, 0);
        assert(!mlist.head);
        assert(!mlist.tail);
    });

});

snow_main(
{
    test_container();
});
