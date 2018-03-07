#define CLIENT_C
#include "client.c"
#undef CLIENT_C

#include <string.h>

int client_fun(struct client *this, void *data, int len)
{
    printf("msglen: %i\n", len);
    printf("server: ");
    for(int i = 0; i < len; i++)
        printf("%c", ((char*)data)[i]);
    if(strcmp(((char*)data), "disconnect") == 0)
        return 0;
    printf("\n");

    return 1;
}

int main()
{
    struct client m_c;

    client_init(&m_c, 0, &client_fun);
    client_connect(&m_c, "127.0.0.1", 5000);
    printf("client connected\n");

    //client_send(&m_c, "testcl", 7);
    //
    char buffer[200];
    while(m_c.alive)
    {
        scanf("%s", buffer);
        int msglen = strlen(buffer);
        printf("buffer: %s\n", buffer);
        printf("len: %i\n", msglen);
        client_send(&m_c, buffer, msglen);
    }

    client_delete(&m_c);

    printf("programm terminating\n");

    return 0;
}
