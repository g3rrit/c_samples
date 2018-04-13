#ifndef HTTP_C
#define HTTP_C
#include "http.c"
#undef HTTP_C
#endif

#include <stdlib.h>

int main()
{
    HTTP_LOG = stdout;

    void *data;
    http_get("en.cppreference.com", "/w/c/memor/realloc", &data);

    FILE *file = fopen("out.txt", "w");
    fprintf(file, "%s", data);
    fclose(file);

    free(data);

    return 0;
}
