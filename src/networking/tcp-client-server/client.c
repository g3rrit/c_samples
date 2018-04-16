//hdr
#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
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

#define THREAD_C
#include "thread.c"
#undef THREAD_C

struct client
{
    struct sockaddr_in address;
    int socket_id;

    struct thread c_thread;
    int alive;

    void *ref;

    int (*fun)(struct client *this, void *data, int len);
};

void client_init(struct client *this, void *ref, int (*fun)(struct client *this, void *data, int len));

int client_connect(struct client *this, char *address, int port);

int client_send(struct client *this, char *buffer, int len);

int client_delete(struct client *this);

#endif

//src
#ifndef CLIENT_C

#include "stdio.h"
#include "string.h"

#define LOG_C
#include "log.c"
#undef LOG_C

void *client_thread_fun(struct client *this)
{
    char buffer[100];
    int msglen;
    this->alive = 1;
    while(this->alive)
    {
        msglen = recv(this->socket_id, buffer, sizeof(char) * 100, 0);

        if(msglen > 0)
            this->alive = this->fun(this, buffer, msglen);
        else
            this->alive = 0;
    }

    return 0;
}

void client_init(struct client *this, void *ref, int (*fun)(struct client *this, void *data, int len))
{
    this->ref = ref;
    this->fun = fun;
}

int client_connect(struct client *this, char *address, int port)
{
    
    int error;
#ifdef WINDOWS
    WSADATA wsa; 
    if(WSAStartup(MAKEWORD(2,2), &wsa))
    {
       log_out("Failed to initialize WSA error: %d\n", WSAGetLastError());
       return -1;
    }

    this->address.sin_addr.s_addr = inet_addr(address);
#else

    struct hostent *dest_id;
    dest_id = gethostbyname(address);
    if(!dest_id)
    {
       log_out("host unknown\n");
       return -1;
    }
    memcpy(&(this->address.sin_addr), dest_id->h_addr, dest_id->h_length);

#endif

    this->address.sin_port = port;
    this->address.sin_family = AF_INET;

    this->socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket_id <= 0)
    {
        log_out("error on socket\n");
        return -2;
    }

    error = connect(this->socket_id, (struct sockaddr*) &(this->address),  sizeof(this->address));
    if(error < 0)
    {
        log_out("error connecting on socket: %i\n", error);
        return -3;
    }

    thread_init(&(this->c_thread));
    thread_create(&(this->c_thread), 0, &client_thread_fun, this);

    return 1;
}

int client_send(struct client *this, char *buffer, int len)
{
    int error = send(this->socket_id, buffer, len, 0);
    if(error < 0)
    {
        log_out("error sending %i\n", error);
        return -1;
    }

    return 1;
}

int client_delete(struct client *this)
{
    void *retval = 0;
    thread_join(&(this->c_thread), retval);

    close(this->socket_id);
    return 1;
}

#endif
