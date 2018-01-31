
#define CONTAINER_C
#define S_LINKED
#include "container.c"
#undef CONTAINER_C

#include "snow.h"

void *m_for_each(int data, void *ref, struct list *_list)
{
    if(data == 5)
    {
        return 5;
    }
    return 0;
}

void *m_for_each_remove(int data, void *ref, struct list *_list)
{
    if(data == 3)
    {
        list_remove_at(_list, 3);
    }
    return 0;
}

void *map_m_for_each(int data, void *ref, struct list *_list)
{
    if(data == 5)
    {
        return 5;
    }
    return 0;
}

void *map_m_for_each_remove(int data, void *ref, struct map *_map)
{
    if(data == 3)
    {
        map_remove_at(_map, 3);
    }
    return 0;
}





describe(container, 
{    
    subdesc(list, 
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

        it("for_each",
        {
            struct list mlist;
            list_init(&mlist);
            for(int i = 0; i < 10; i++)
            {
                list_push_back(&mlist, i);
                asserteq(mlist.size, i+1);
                asserteq(list_at(&mlist,9) , i);
            }
            asserteq(list_for_each(&mlist, &m_for_each, 0), 5);
            asserteq(list_for_each(&mlist, &m_for_each_remove, 0), 0);
            asserteq(mlist.size, 9);
            assert(mlist.head);
            assert(mlist.tail);
        });

    });

    subdesc(map, 
    {
        it("push_front", 
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_front(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,0) , i);
                (*key)++;
            }        
            asserteq(map_get(&mmap, "ckey"), 2);
            asserteq(mmap.size, 10);
            free(key);
        });

        it("push_back", 
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap,key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
                (*key)++;
            }        
            asserteq(map_get(&mmap, "bkey"), 1);
            asserteq(mmap.size, 10);
            free(key);
        });

        it("map_delete", 
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_front(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,0) , i);
                (*key)++;
            } 
            asserteq(map_get(&mmap, "dkey"), 3);
            map_delete(&mmap);
            assert(!mmap.size);
            assert(!mmap.head);
            assert(!mmap.tail);
            free(key);
        });

        it("pop_back",
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
                (*key)++;
            }
            for(int i = 9; i >= 0; i--)
            {
                asserteq(map_pop_back(&mmap), i);
            }
            asserteq(map_get(&mmap, "akey"), 0);
            assert(!mmap.size);
            assert(!mmap.head);
            assert(!mmap.tail);
            free(key);
        });

        it("pop_front",
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
                (*key)++;
            }
            for(int i = 0; i < 10; i++)
            {
                asserteq(map_pop_front(&mmap), i);
            }
            //returns 0 pointer
            asserteq(map_get(&mmap, "ekey"), 0);
            assert(!mmap.size);
            assert(!mmap.head);
            assert(!mmap.tail);
            free(key);
        });

        it("at",
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
                (*key)++;
            }
            for(int i = 0; i < 10; i++)
            {
                asserteq(map_at(&mmap, i), i);
            }
            asserteq(map_get(&mmap, "fkey"), 5);
            asserteq(mmap.size, 10);
            assert(mmap.head);
            assert(mmap.tail);
            free(key);
        });

        it("remove_at",
        {
            struct map mmap;
            map_init(&mmap);
            char *key = malloc(5);
            strcpy(key, "akey");
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap, key, i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
                (*key)++;
            }
            asserteq(map_get(&mmap, "gkey"), 6);
            asserteq(map_remove_at(&mmap, 0), 0);
            asserteq(map_remove_at(&mmap, 8), 9);
            asserteq(map_remove_at(&mmap, 3), 4);
            asserteq(map_remove_at(&mmap, 5), 7);
            asserteq(map_remove_at(&mmap, 4), 6);
            asserteq(map_remove_at(&mmap, 4), 8);
            asserteq(map_remove_at(&mmap, 1), 2);
            asserteq(map_remove_at(&mmap, 0), 1);
            asserteq(map_remove_at(&mmap, 1), 5);
            asserteq(map_remove_at(&mmap, 0), 3);

            asserteq(mmap.size, 0);
            assert(!mmap.head);
            assert(!mmap.tail);
            free(key);
        });

        it("for_each",
        {
            struct map mmap;
            map_init(&mmap);
            for(int i = 0; i < 10; i++)
            {
                map_push_back(&mmap, "key", i);
                asserteq(mmap.size, i+1);
                asserteq(map_at(&mmap,9) , i);
            }
            asserteq(map_for_each(&mmap, &map_m_for_each, 0), 5);
            asserteq(map_for_each(&mmap, &map_m_for_each_remove, 0), 0);
            asserteq(mmap.size, 9);
            assert(mmap.head);
            assert(mmap.tail);
        });

    });


});

snow_main(
{
    test_container();
});
