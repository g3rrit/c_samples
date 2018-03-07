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

void server_remove_client(struct server *this, char *id);

void discard_client(struct client_info *client);

int server_delete(struct server *this);

#endif

//src
#ifndef SERVER_C

#include "stdio.h"

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
            info->alive = info->fun(info, buffer, msglen);
        else
            info->alive = 0;
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
       printf("Failed to initialize WSA error: %d\n", WSAGetLastError());
       return -1;
   }
#endif

    char buffer[200];
    this->socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(!this->socket_id)
    {
        printf("error setting up socket\n");
        return -2;
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;    //accept all
    this->address.sin_port = port;

    int error = bind(this->socket_id, (struct sockaddr*) &(this->address), sizeof(this->address));
    if(error < 0)
    {
        printf("error binding to socket: %i\n", error);
        return -3;
    }

    error = listen(this->socket_id, 5);       //listen(int s, int backlog);
    if(error <  0)
    {
        printf("error listening on socket: %i\n", error);
        return -4;
    }

    printf("server initialized [%i]\n", port);

    return 1;
}

struct client_info *server_listen(struct server *this)
{
    // joint all threads in join_vector
    
    struct client_info *rem_client;
    printf("join vec size: %i\n", this->join_vec.size);
    for(int i = 0; i < this->join_vec.size; i++)
    {
        rem_client = vector_at(&(this->join_vec), i);
        map_remove(&(this->c_map), rem_client->id);
        void *retv = 0;
        thread_join(&(rem_client->c_thread), retv);
        discard_client(rem_client);
    }
    // -----
    
    printf("listening for connections\n");
    //socklen_t instead of int
    int addrlen = sizeof(struct sockaddr);
    struct client_info *temp_client = malloc(sizeof(struct client_info));

    printf("accepting connections\n");

    temp_client->socket_id = accept(this->socket_id, (struct sockaddr*) &(temp_client->address), &addrlen);

    printf("accepted connection\n");
    /*int msglen = recv(socket_c_id, buffer, 200, 0);
    for(int i = 0; i < msglen; i++)
        printf("%c", buffer[i]);


    for(int i = 0; i < 200; i++)
        buffer[i] = 'B';
    send(socket_c_id, buffer, 200, 0);

    close(socket_c_id);
    */


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
}

void server_remove_client(struct server *this, char *id)
{
    struct client_info *info = map_get(&(this->c_map), id);
    info->alive = 0;
    void *retv = 0;
    thread_join(&(info->c_thread), retv);

    map_remove(&(this->c_map), id);
    discard_client(info);
}

void discard_client(struct client_info *client)
{
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
    return 1;
}

#endif
