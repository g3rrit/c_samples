//hdr
#ifndef HTTP_H
#define HTTP_H

//return length of data returned from request
int http_get(char *host, char *url, void **data);

#include <stdio.h>

//set this if you want to get logs
extern FILE *HTTP_LOG;

#endif

//src
#ifndef HTTP_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

FILE *HTTP_LOG = 0;

int r_alloc(void **data, int size);

int http_get(char *host, char *url, void **data)
{
    struct hostent *server;
    struct sockaddr_in server_addr;
    int port = 80;
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(tcp_socket == -1)
    {
        printf("-----!error opening tcp_socket!-----\n");
        return 0;
    }

    if(HTTP_LOG)
        fprintf(HTTP_LOG, "+ tcp socket succesfully opened\n");

    if((server = gethostbyname(host)) == 0)
    {
        close(tcp_socket);
        printf("-----!error getting hostname!-----\n");
        return 0; 
    }

    if(HTTP_LOG)
        fprintf(HTTP_LOG, "%s -----------\n", server->h_name);
    
    /*if(HTTP_LOG)
    {
        for(int i = 0; server->h_addr_list[i]; i++)
            fprintf(HTTP_LOG, " -> %s\n", inet_ntoa(*(struct in_addr*)(server->h_addr_list[i]))); 
    }
    */

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    bcopy(server->h_addr, &server_addr.sin_addr.s_addr, server->h_length);

    server_addr.sin_port = htons(port);

    if(connect(tcp_socket, &server_addr, sizeof(server_addr)) < 0)
    {
        printf("-----!error connection to server!-----\n");
        return 0;
    }

    if(HTTP_LOG)
        fprintf(HTTP_LOG, "succefully connected to server\n");

#define REQ_BUF_S 30
    int request_size = sizeof(host) + sizeof(url) + 30 + REQ_BUF_S;
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "Get %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);

    if(HTTP_LOG)
        fprintf(HTTP_LOG, "request:\n%s\n", request);

    /*if(send(tcp_socket, request, strlen(request), 0) < 0)
    {
        printf("-----!error sending request!-----\n");
        return 0;
    }
    */

    //send request 
    int sent_bytes = 0;
    int total_bytes = strlen(request);
    while(sent_bytes < total_bytes)
    {
        int bytes = write(tcp_socket, request + sent_bytes, total_bytes - sent_bytes);
        if(bytes < 0)
        {
            printf("-----!error sending bytes: %i!-----\n", sent_bytes);
            return 0;
        }
        if(bytes == 0)
            break;

        sent_bytes += bytes;
    }

    if(HTTP_LOG)
        fprintf(HTTP_LOG, "succesfully sent request\n");

    //receive response
#define UPPER_BUFF_S 32768
    int recv_bytes = 0;
    int buffer_size = 4096;
    *data = malloc(buffer_size);
    while(recv_bytes < UPPER_BUFF_S)
    {
        int bytes = read(tcp_socket, *data + recv_bytes, buffer_size);
        if(bytes < 0)
        {
            printf("-----!error receiving response!-----\n");
            return 0;
        }
        if(bytes == 0)
            break;
        recv_bytes += bytes;
        if(recv_bytes >= buffer_size)
            r_alloc(data, buffer_size);
    }

    *data = realloc(*data, recv_bytes + 1);  
    ((char*)*data)[recv_bytes] = 0;

    if(HTTP_LOG)
    {
        fprintf(HTTP_LOG, "received %i bytes\n", recv_bytes);
        fprintf(HTTP_LOG, "response:\n%s\n", *data);
    }

    close(tcp_socket);

    return recv_bytes + 1;
}

int r_alloc(void **data, int size)
{
    int new_size = (size * 2)/3;
    *data = realloc(*data, new_size);
    
    if(new_size > size)
        bzero(*data + size, new_size - size);
    return new_size;
}

#endif
