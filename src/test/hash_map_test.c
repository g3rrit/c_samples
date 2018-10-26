

#include "hash_map.h"
#include "snow.h"

#define map_size 2

void *for_each(char *key, char *data, struct hash_map_t *ref) {
    hash_map_remove(ref, key);
    return 0;
}

describe(hash_map, {

        it("insert", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 4);

            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);
        });

        it("get", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key2", "datat");
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 4);

            assert(!strcmp(hash_map_get(&map, "key1"), "data1"));
            assert(!strcmp(hash_map_get(&map, "key2"), "data2"));
            assert(!strcmp(hash_map_get(&map, "key3"), "data3"));
            assert(hash_map_remove(&map, "key2"));
            assert(!strcmp(hash_map_get(&map, "key2"), "datat"));

            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);

        });

        it("remove", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 3);

            assert(hash_map_remove(&map, "key2"));
            asserteq(hash_map_size(&map), 2);
            assert(hash_map_remove(&map, "key3"));
            asserteq(hash_map_size(&map), 1);
            assert(!hash_map_remove(&map, "key3"));
            asserteq(hash_map_size(&map), 1);

            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);
        });

        it("for_each", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 3);

            assert(!hash_map_for_each(&map, &for_each, &map));
            asserteq(hash_map_size(&map), 0);
            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);
        });

        it("delete", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 4);

            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);
            hash_map_init(&map, map_size);

            asserteq(hash_map_size(&map), 0);
            hash_map_insert(&map, "key1", "data1");
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key2", "data2");
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key3", "data3");
            asserteq(hash_map_size(&map), 4);

            hash_map_delete(&map);
            asserteq(hash_map_size(&map), 0);
        });

        it("delete_all", {
            struct hash_map_t map;
            hash_map_init(&map, map_size);

            hash_map_insert(&map, "key1", malloc(234)); 
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", malloc(234)); 
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key3", malloc(234)); 
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key4", malloc(234)); 
            asserteq(hash_map_size(&map), 4);
            hash_map_insert(&map, "key5", malloc(234)); 
            asserteq(hash_map_size(&map), 5);

            hash_map_delete_all(&map);
            hash_map_init(&map, map_size);

            hash_map_insert(&map, "key1", malloc(234)); 
            asserteq(hash_map_size(&map), 1);
            hash_map_insert(&map, "key2", malloc(234)); 
            asserteq(hash_map_size(&map), 2);
            hash_map_insert(&map, "key3", malloc(234)); 
            asserteq(hash_map_size(&map), 3);
            hash_map_insert(&map, "key4", malloc(234)); 
            asserteq(hash_map_size(&map), 4);
            hash_map_insert(&map, "key5", malloc(234)); 
            asserteq(hash_map_size(&map), 5);
            hash_map_delete_all(&map);
        });
});


snow_main();

    
