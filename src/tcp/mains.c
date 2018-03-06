#define SERVER_C
#include "server.c"
#undef SERVER_C

int client_fun(struct client_info *info, void *data, size_t len)
{
    printf("client[%s]: \n", info->id);
    for(int i = 0; i < len; i++)
        printf("%c", ((char*)data)[i]);

    return 1;
}

int main()
{
    struct server s;
    server_init(&s, "127.0.0.1", 5000);

    char *t_id = malloc(sizeof(2)); 
    t_id[0] = '0';
    t_id[1] = 0;

    while(1)
    {
        printf("listening for clients\n");
        struct client_info *p_cl = server_listen(&s);
        printf("adding client to servermap\n");
        server_add_client(&s, p_cl, t_id, 0, &client_fun);
        t_id[0]++;       
    }

    free(t_id);

    server_delete(&s);

    return 0;
}
