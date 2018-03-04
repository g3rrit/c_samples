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


struct client_info
{
    char *id;
    void *ref;
    void (*fun)(struct client_info *info, void *data, size_t len);
};

struct server
{
    int socket_id;
    struct sockaddr_in address;
    void (*fun)(struct client_info *info, void *data, size_t len);
};

int server_init(struct server *this, char *address, int port);

int server_close(struct server *this);

#endif

//src
#ifndef SERVER_C

#include "stdio.h"

struct sockaddr_in this->address;
int this->socket_id;

int server_init(struct server *this, char *address, int port)
{
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

    struct sockaddr_in sock_p;
    //socklen_t instead of int
    int addrlen = sizeof(struct sockaddr);

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

    printf("listening for connections\n");
    while(1)
    {
        int socket_c_id = accept(this->socket_id, (struct sockaddr*) &sock_p, &addrlen);
        printf("socket_c_id %i\n", socket_c_id);
        printf("accepted connection\n");
        int msglen = recv(socket_c_id, buffer, 200, 0);
        for(int i = 0; i < msglen; i++)
            printf("%c", buffer[i]);


        for(int i = 0; i < 200; i++)
            buffer[i] = 'B';
        send(socket_c_id, buffer, 200, 0);
        close(socket_c_id);
    }

    return 1;
}

int server_close(struct server *this)
{
    close(this->socket_id);
    return 1;
}

#endif
