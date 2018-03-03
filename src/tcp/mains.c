#define SERVER_C
#include "server.c"
#undef SERVER_C

int main()
{
    server_init("127.0.0.1", 5000);
    server_close();

    return 0;
}
