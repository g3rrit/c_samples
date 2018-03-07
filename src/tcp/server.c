//hdr
#ifndef SERVER_H
#define SERVER_H

#if defined _WIN32 || defined _WIN64
#define WINDOWS
#include <winsock2.h>
//link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/signal.h>
#include <unistd.h>
#endif

#define CONTAINER_C
#include "container.c"
#undef CONTAINER_C

#define THREAD_C
#include "thread.c"
#undef THREAD_C

struct client_info
{
    char *id;
    void *ref;
    //if this function returns 0 the connetion closes
    int (*fun)(struct client_info *info, void *data, int len);
    int alive;

    int socket_id;
    struct sockaddr_in address;
    struct thread c_thread;
    struct server *s_info;
};

struct server
{
    int socket_id;
    struct sockaddr_in address;

    struct map c_map;
    struct vector join_vec;
};

int server_init(struct server *this, char *address, int port);

struct client_info *server_listen(struct server *this);

void server_add_client(struct server *this, struct client_info *client, char *id, void *ref, int (*fun)(struct client_info *info, void *data, int len));

int server_send(struct server *this, char *client_id, void *data, int len);

void server_broadcast(struct server *this, void *data, int len);

void server_remove_client(struct server *this, char *id);

void discard_client(struct client_info *client);

int server_delete(struct server *this);

#endif

//src
#ifndef SERVER_C

#define LOG_C
#include "log.c"
#undef LOG_C

//s_r = 1 -> data send
//s_r = 0 -> data received
void log_data_transfered(int s_r, char *c_id, void *data, int len);

void *client_thread_fun(struct client_info *info)
{
    char buffer[100];
    int msglen = 0;
    info->alive = 1;
    while(info->alive)
    {
        //int msglen = recv(socket_c_id, buffer, 200, 0);
        msglen = recv(info->socket_id, buffer, sizeof(char) * 100, 0);
        if(msglen > 0)
        {
            log_data_transfered(0, info->id, buffer, msglen);
            info->alive = info->fun(info, buffer, msglen);
        }
        else
        {
            info->alive = 0;
        }
    }

    vector_push_back(&(info->s_info->join_vec), info);

    return 0;
}

int server_init(struct server *this, char *address, int port)
{
    map_init(&(this->c_map));
    vector_init(&(this->join_vec), sizeof(struct client_info*), 10);

#ifdef WINDOWS
   WSADATA wsa; 
   if(WSAStartup(MAKEWORD(2,2), &wsa))
   {
       log_out("Failed to initialize WSA error: %d\n", WSAGetLastError());
       return -1;
   }
#endif

    char buffer[200];
    this->socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->socket_id)
    {
        log_out("error setting up socket\n");
        return -2;
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;    //accept all
    this->address.sin_port = port;

    int error = bind(this->socket_id, (struct sockaddr*) &(this->address), sizeof(this->address));
    if(error < 0)
    {
        log_out("error binding to socket: %i\n", error);
        return -3;
    }

    error = listen(this->socket_id, 5);       //listen(int s, int backlog);
    if(error <  0)
    {
        log_out("error listening on socket: %i\n", error);
        return -4;
    }

    log_out("server initialized on[%s:%i]\n",address, port);

    return 1;
}

struct client_info *server_listen(struct server *this)
{
    // joint all threads in join_vector
    
    struct client_info *rem_client;
    for(int i = 0; i < this->join_vec.size; i++)
    {
        rem_client = vector_at(&(this->join_vec), i);
        map_remove(&(this->c_map), rem_client->id);
        void *retv = 0;
        thread_join(&(rem_client->c_thread), retv);
        discard_client(rem_client);
    }
    // -----
    
    log_out("listening for connections\n");
    //socklen_t instead of int
    int addrlen = sizeof(struct sockaddr);
    struct client_info *temp_client = malloc(sizeof(struct client_info));

    temp_client->socket_id = accept(this->socket_id, (struct sockaddr*) &(temp_client->address), &addrlen);

    log_out("accepted connection\n");

    //reinit join vector
    vector_delete(&(this->join_vec));

    return temp_client;
}

void server_add_client(struct server *this, struct client_info *client, char *id, void *ref, int (*fun)(struct client_info *info, void *data, int len))
{
    client->fun = fun;
    client->id = id;
    client->ref = ref;
    client->s_info = this;

    map_push_back(&(this->c_map), id, client);

    thread_init(&(client->c_thread));
    thread_create(&(client->c_thread), 0, &client_thread_fun, client);

    log_out("client[%s] added to server map\n", id);
}

int server_send(struct server *this, char *client_id, void *data, int len)
{
    struct client_info *info = map_get(&(this->c_map), client_id);
    int errorlen = send(info->socket_id, data, len, 0);
    if(errorlen != len)
    {
        log_out("failed to send complete data: %i\n", errorlen);
        return 0;
    } 
    else
    {
        log_data_transfered(1, info->id, data, len);
        return 1;
    }
}

struct server_broadcast_ref
{
    struct server *this;
    void *data;
    int len;
};

void *server_broadcast_for_each(void *data, void *rref, struct map_info *info)
{
    struct server_broadcast_ref *ref = rref;
    struct server *this = ref->this;
    struct client_info *c_info = data;

    int errorlen = send(c_info->socket_id, ref->data, ref->len, 0);
    if(errorlen != ref->len)
    {
        log_out("failed to broadcast to client[%s] error: %i\n", c_info->id, ref->len);
    }
    else
    {
        log_data_transfered(1, c_info->id, ref->data, ref->len);
    }

    return 0;
}

void server_broadcast(struct server *this, void *data, int len)
{
    struct server_broadcast_ref b_ref;
    b_ref.this = this;
    b_ref.data = data;
    b_ref.len = len;

    map_for_each(&(this->c_map), &server_broadcast_for_each, &b_ref);
}

void server_remove_client(struct server *this, char *id)
{
    struct client_info *info = map_get(&(this->c_map), id);
    info->alive = 0;
    void *retv = 0;
    thread_join(&(info->c_thread), retv);

    map_remove(&(this->c_map), id);
    discard_client(info);

    log_out("client[%s] remove from server map\n", id);
}

void discard_client(struct client_info *client)
{
    log_out("closing connection to client[%s]\n", client->id);

    close(client->socket_id);
    free(client);
}

int server_delete(struct server *this)
{
    for(int i = 0; i < this->c_map.size; i++)
    {
        struct client_info *info = map_at(&(this->c_map), i);
        info->alive = 0;
        void *retv = 0;
        thread_join(&(info->c_thread), retv);
        discard_client(info);
    }
     
    map_delete(&(this->c_map));
    close(this->socket_id);

    log_out("server closed\n");

    return 1;
}

void log_data_transfered(int s_r, char *c_id, void *data, int len)
{
    if(s_r)
    {
        //data send
        log_out("sending data to client[%s]\n", c_id);
        log_out("msglen: %i\n[:", len);
        for(int i = 0; i < len; i++)
            printf("%hhX:", ((char*)data)[i]);
        log_out("]\n");
    }
    else
    {
        //data received
        log_out("receiving data from client[%s]\n", c_id);
        log_out("msglen: %i\n[:", len);
        for(int i = 0; i < len; i++)
            printf("%hhX:", ((char*)data)[i]);
        log_out("]\n");
    }
}

#endif
