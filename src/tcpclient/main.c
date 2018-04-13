#ifndef TCP_C
#define TCP_C
#include "tcp.c"
#undef TCP_C
#endif

#include <stdlib.h>

int main()
{
    TCP_LOG = stdout;

    void *data;
    http_get("en.cppreference.com", "/w/c/memor/realloc", &data);

    FILE *file = fopen("out.txt", "w");
    fprintf(file, "%s", data);
    fclose(file);

    free(data);

    http_post("man7.org", "/linux/man-pages/man2/socket.2.html", "", &data);

    free(data);

    return 0;
}
