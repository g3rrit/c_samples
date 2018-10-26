#include "hash_map.h"
#include "hash_tree.h"
#include "snow.h"

void for_each_test(char *data, void *ref, char **keys) {
    char *key = keys[0];
    int i = 0;
    while(key) {
        i++;
        printf("k: %s | ", key);
        key = keys[i];
    }
    printf("\ndata: %s\n", data);
}

#define TREE_SIZE 16
describe(hash_tree, {
    it("insert", {
        struct hash_tree_t tree;
        hash_tree_init(&tree, TREE_SIZE);

        asserteq(hash_tree_size(&tree), 0);

        void *res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 1);
        res = hash_tree_insert(&tree, "data112", "k1", "k1", "k2", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 2);
        res = hash_tree_insert(&tree, "data123", "k1", "k2", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 3);
        res = hash_tree_insert(&tree, "data133", "k1", "k3", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 4);
        res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        assert(res);
        asserteq(hash_tree_size(&tree), 4);

        hash_tree_delete(&tree);
        asserteq(hash_tree_size(&tree), 0);
    });

    it("get", {
        struct hash_tree_t tree;
        hash_tree_init(&tree, TREE_SIZE);

        asserteq(hash_tree_size(&tree), 0);

        void *res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 1);
        res = hash_tree_insert(&tree, "data112", "k1", "k1", "k2", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 2);
        res = hash_tree_insert(&tree, "data123", "k1", "k2", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 3);
        res = hash_tree_insert(&tree, "data133", "k1", "k3", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 4);
        res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        assert(res);
        asserteq(hash_tree_size(&tree), 4);

        res = hash_tree_get(&tree, "k1", "k2", "k3", 0);
        assert(!strcmp(res, "data123"));

        hash_tree_delete(&tree);
        asserteq(hash_tree_size(&tree), 0);
    });

    it("remove", {
        struct hash_tree_t tree;
        hash_tree_init(&tree, TREE_SIZE);

        asserteq(hash_tree_size(&tree), 0);

        void *res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 1);
        res = hash_tree_insert(&tree, "data112", "k1", "k1", "k2", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 2);
        res = hash_tree_insert(&tree, "data123", "k1", "k2", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 3);
        res = hash_tree_insert(&tree, "data133", "k1", "k3", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 4);
        res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        assert(res);
        asserteq(hash_tree_size(&tree), 4);

        res = hash_tree_remove(&tree, "k1", "k1", "k2", 0);
        assert(!strcmp(res, "data112"));

        asserteq(hash_tree_size(&tree), 3);

        hash_tree_delete(&tree);
        asserteq(hash_tree_size(&tree), 0);

    });

    it("for_each"; {
        struct hash_tree_t tree;
        hash_tree_init(&tree, TREE_SIZE);

        asserteq(hash_tree_size(&tree), 0);

        void *res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 1);
        res = hash_tree_insert(&tree, "data112", "k1", "k1", "k2", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 2);
        res = hash_tree_insert(&tree, "data123", "k1", "k2", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 3);
        res = hash_tree_insert(&tree, "data133", "k1", "k3", "k3", 0);
        asserteq(res, 0);
        asserteq(hash_tree_size(&tree), 4);
        res = hash_tree_insert(&tree, "data111", "k1", "k1", "k1", 0);
        assert(res);
        asserteq(hash_tree_size(&tree), 4);

        hash_tree_for_each(&tree, (hash_tree_for_each_f)&for_each_test, 0);

        hash_tree_delete(&tree);
        asserteq(hash_tree_size(&tree), 0);

    });
});

snow_main();

