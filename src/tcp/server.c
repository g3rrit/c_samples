//hdr
#ifndef SERVER_H
#define SERVER_H

int server_init(char *address, int port);

int server_close();

#endif

//src
#ifndef SERVER_C

#if defined _WIN32 || defined _WIN64
#define WINDOWS
#include <winsock2.h>
//ling with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/signal.h>
#include <unistd.h>
#endif

#include "stdio.h"

struct sockaddr_in sock_a;
int socket_id;

int server_init(char *address, int port)
{
#ifdef WINDOWS
   WSADATA wsa; 
   if(!WSAStartup(MAKEWORD(2,2), &wsa))
   {
       printf("Failed to initialize WSA error: %d\n", WSAGetLastError());
       return -1;
   }
#endif

    char buffer[200];
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(!socket_id)
    {
        printf("error setting up socket\n");
        return -2;
    }

    sock_a.sin_family = AF_INET;
    sock_a.sin_addr.s_addr = INADDR_ANY;    //accept all
    sock_a.sin_port = port;

    struct sockaddr_in sock_p;
    socklen_t addrlen = sizeof(struct sockaddr);

    int error = bind(socket_id, (struct sockaddr*) &sock_a, sizeof(sock_a));
    if(error < 0)
    {
        printf("error binding to socket: %i\n", error);
        return -3;
    }

    error = listen(socket_id, 5);       //listen(int s, int backlog);
    if(error <  0)
    {
        printf("error listening on socket: %i\n", error);
        return -4;
    }

    while(1)
    {
        int socket_c_id = accept(socket_id, (struct sockaddr*) &sock_p, &addrlen);
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

int server_close()
{
    close(socket_id);
    return 1;
}

#endif
