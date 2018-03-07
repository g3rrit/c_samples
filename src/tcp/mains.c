#define SERVER_C
#include "server.c"
#undef SERVER_C

#define THREAD_C
#include "thread.c"
#undef THREAD_C

#include <string.h>

int client_fun(struct client_info *info, void *data, int len)
{
    printf("msglen: %i\n", len);
    printf("client[%s]: ", info->id);
    for(int i = 0; i < len; i++)
        printf("%c", ((char*)data)[i]);
    if(strcmp(((char*)data), "disconnect") == 0)
        return 0;
    printf("\n");

    return 1;
}

void *server_talk_fun(struct server *this)
{
    char buffer[200];
    buffer[0] = 'o';
    while(buffer[0] != 'q')
    {
        scanf("%s", buffer);
        int msglen = strlen(buffer);
        printf("buffer: %s\n", buffer);
        printf("len: %i\n", msglen);
        server_broadcast(this, buffer, msglen);
    }

    return 0;
}

int main()
{
    struct server s;
    server_init(&s, "127.0.0.1", 5000);

    char *t_id = malloc(sizeof(2)); 
    t_id[0] = '0';
    t_id[1] = 0;

    struct thread s_thread;
    thread_init(&s_thread);
    thread_create(&s_thread, 0, &server_talk_fun, &s);

    while(1)
    {
        struct client_info *p_cl = server_listen(&s);
        server_add_client(&s, p_cl, t_id, 0, &client_fun);
        t_id[0]++;       
    }

    void *rval = 0;
    thread_join(&s_thread, rval);

    free(t_id);

    server_delete(&s);

    return 0;
}
