#include "hash_map.h"

#include <stdio.h>
#include <inttypes.h>

int test_hash() {

    char line[1024] = {0};

    for(;;) {
        scanf("%1023[^\n]", line);

        printf("position of %s is: %i \n", line, get_position(line, 64));

        memset(line, 0, 1024);
        getchar();
    }

    return 0;
}

void *print_map(char *data, char *key, void *ref) {
    printf("for: key: %s data: %s\n", key, data);
    return 0;
}

int test_map() {
    
    struct hash_map_t map;
    hash_map_init(&map, 5);

    printf("hash_map inited\n");

    hash_map_insert(&map, "key1" , "data1");
    printf("first elem\n");
    hash_map_insert(&map, "key2" , "data2");
    printf("elem\n");
    hash_map_insert(&map, "key3" , "data3");
    printf("elem\n");
    hash_map_insert(&map, "key4" , "data4");
    printf("elem\n");
    hash_map_insert(&map, "key5" , "data5");
    printf("elem\n");
    hash_map_insert(&map, "key6" , "data6");
    printf("elem\n");
    hash_map_insert(&map, "key7" , "data7");
    printf("elem\n");
    hash_map_insert(&map, "key8" , "data8");
    printf("elem\n");
    hash_map_insert(&map, "key9" , "data9");
    printf("elem\n");
    hash_map_insert(&map, "key10" , "data10");
    printf("elem\n");

    printf("all elem\n");

#define print_get(key) if(hash_map_get(&map, key)) printf("got: %s\n", hash_map_get(&map, key)); else printf("empty\n");
#define print_remove(key) if(hash_map_get(&map, key)) printf("got: %s\n", hash_map_remove(&map, key)); else printf("empty\n");

    print_get("key3");
    print_get("key4");
    print_get("key5");
    print_get("key30");
    print_get("key30");
    print_get("keyd0");
    print_get("kes30");
    print_get("keyd0");

    print_remove("key3");
    print_remove("key4");
    print_remove("key5");
    print_remove("key30");
    print_remove("key30");
    print_remove("keyd0");
    print_remove("kes30");
    print_remove("keyd0");

    print_get("key1");
    print_get("key2");
    print_get("key3");
    print_get("key4");
    print_get("key5");
    print_get("key6");
    print_get("key7");
    print_get("key8");
    print_get("key9");
    print_get("key10");

    hash_map_for_each(&map, &print_map, 0);

    hash_map_delete(&map);
    hash_map_init(&map, 10);

    hash_map_for_each(&map, &print_map, 0);

    return 0;
}

int main() {
    test_map();
    return 0;
}


