#define SERVER_C
#include "server.c"
#undef SERVER_C

int main()
{
    struct server s;
    server_init(&s, "127.0.0.1", 5000);
    server_close(&s);

    return 0;
}
