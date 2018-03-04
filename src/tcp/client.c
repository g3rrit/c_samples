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

struct client
{
    struct sockaddr_in address;
    int socket_id;
};

int client_connect(struct client *this, char *address, int port);

int client_send(struct client *this, char *buffer, size_t len);

int client_delete(struct client *this);

#endif

//src
#ifndef CLIENT_C

#include "stdio.h"
#include "string.h"

int client_connect(struct client *this, char *address, int port)
{
    
    int error;
#ifdef WINDOWS
    WSADATA wsa; 
    if(SAStartup(MAKEWORD(2,2), &wsa))
    {
       printf("Failed to initialize WSA error: %d\n", WSAGetLastError());
       return -1;
    }

    this->address.sin_addr.s_addr = inet_addr(address);
#else

    struct hostent *dest_id;
    dest_id = gethostbyname(address);
    if(!dest_id)
    {
       printf("host unknown\n");
       return -1;
    }
    memcpy(&(this->address.sin_addr), dest_id->h_addr, dest_id->h_length);

#endif

    this->address.sin_port = port;
    this->address.sin_family = AF_INET;

    this->socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket_id <= 0)
    {
        printf("error on socket\n");
        return -2;
    }

    error = connect(this->socket_id, (struct sockaddr*) &(this->address),  sizeof(this->address));
    if(error < 0)
    {
        printf("error connecting on socket: %i\n", error);
        return -3;
    }


    return 1;
}

int client_send(struct client *this, char *buffer, size_t len)
{
    int error = send(this->socket_id, buffer, len, 0);
    if(error < 0)
    {
        printf("error sending %i\n", error);
        return -1;
    }
    char rbuffer[200];
    int rlen = recv(this->socket_id, rbuffer, 200, 0);
    for(int i = 0; i < 200; i++)
        printf("%c", rbuffer[i]);

    return 1;
}

int client_delete(struct client *this)
{
    close(this->socket_id);
    return 1;
}

#endif
