#include "hash_map.h"

#include <stdint.h>
#include <string.h>

uint32_t get_position(char *name, uint32_t max_size) {
    uint8_t res_hash[4] = { 0x33, 0x33, 0x33, 0x33 };
    int hash_pos = 0;
    int name_len = strlen(name);

    for(int i = 0; i < name_len; i++) {
        res_hash[hash_pos] ^= (uint8_t)name[i];
        hash_pos++;
        hash_pos %= 3;
    }

    uint32_t res = 0;
    memcpy(&res, res_hash, 4);

    return res % max_size;
}





#ifdef DEBUG_TEST

#include <stdio.h>
#include <inttypes.h>

int test() {

    char line[1024] = {0};

    for(;;) {
        scanf("%1023[^\n]", line);

        printf("position of %s is: %i \n", line, get_position(line, 64));

        memset(line, 0, 1024);
        getchar();
    }

    return 0;
}

#endif

