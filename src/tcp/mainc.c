#define CLIENT_C
#include "client.c"
#undef CLIENT_C

int main()
{
    struct client m_c;

    client_connect(&m_c, "127.0.0.1", 5000);

    //client_send(&m_c, "testcl", 7);

    client_delete(&m_c);

    return 0;
}
