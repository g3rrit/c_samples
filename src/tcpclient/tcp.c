//hdr
#ifndef TCP_H
#define TCP_H

#include <netinet/in.h>

struct tcp_connection
{
    struct hostent *server;
    struct sockaddr_in server_addr;
    int port;
    int tcp_socket;
};

int tcp_connect(struct tcp_connection *tcp_conn, char *host, int port);

int tcp_send(struct tcp_connection *tcp_conn, char *request);

int tcp_recv(struct tcp_connection *tcp_conn, char **data);

int tcp_close(struct tcp_connection *tcp_conn);

//HTTP

//return length of data returned from request
int http_get(char *host, char *url, void **data);

int http_post(char *host, char *url, char *query_string, void **data);


#include <stdio.h>

//set this if you want to get logs
extern FILE *TCP_LOG;

#endif

//src
#ifndef TCP_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

FILE *TCP_LOG = 0;

int r_alloc(void **data, int size);

int int_s_length(int val);

int tcp_connect(struct tcp_connection *tcp_conn, char *host, int port)
{
    tcp_conn->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    tcp_conn->port = port;

    if(tcp_conn->tcp_socket == -1)
    {
        printf("-----!error opening tcp_socket!-----\n");
        return 0;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "+ tcp socket succesfully opened\n");

    if((tcp_conn->server = gethostbyname(host)) == 0)
    {
        close(tcp_conn->tcp_socket);
        printf("-----!error getting hostname!-----\n");
        return 0; 
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "%s -----------\n", tcp_conn->server->h_name);
    
    /*if(TCP_LOG)
    {
        for(int i = 0; tcp_conn->server->h_addr_list[i]; i++)
            fprintf(TCP_LOG, " -> %s\n", inet_ntoa(*(struct in_addr*)(tcp_conn->server->h_addr_list[i]))); 
    }
    */

    bzero(&tcp_conn->server_addr, sizeof(tcp_conn->server_addr));

    tcp_conn->server_addr.sin_family = AF_INET;

    bcopy(tcp_conn->server->h_addr, &tcp_conn->server_addr.sin_addr.s_addr, tcp_conn->server->h_length);

    tcp_conn->server_addr.sin_port = htons(tcp_conn->port);

    if(connect(tcp_conn->tcp_socket, &tcp_conn->server_addr, sizeof(tcp_conn->server_addr)) < 0)
    {
        printf("-----!error connection to server!-----\n");
        return 0;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "succefully connected to server\n");

    return 1;
}

int tcp_send(struct tcp_connection *tcp_conn, char *request)
{
    if(TCP_LOG)
        fprintf(TCP_LOG, "request:\n%s\n", request);

    //send request 
    int sent_bytes = 0;
    int total_bytes = strlen(request);
    while(sent_bytes < total_bytes)
    {
        int bytes = write(tcp_conn->tcp_socket, request + sent_bytes, total_bytes - sent_bytes);
        if(bytes < 0)
        {
            printf("-----!error sending bytes: %i!-----\n", sent_bytes);
            return 0;
        }
        if(bytes == 0)
            break;

        sent_bytes += bytes;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "succesfully sent request\n");

    return total_bytes;
}

int tcp_recv(struct tcp_connection *tcp_conn, char **data)
{
    //receive response
#define UPPER_BUFF_S 32768
    int recv_bytes = 0;
    int buffer_size = 4096;
    *data = malloc(buffer_size);
    while(recv_bytes < UPPER_BUFF_S)
    {
        int bytes = read(tcp_conn->tcp_socket, *data + recv_bytes, buffer_size);
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

    if(TCP_LOG)
    {
        fprintf(TCP_LOG, "received %i bytes\n", recv_bytes);
        fprintf(TCP_LOG, "response:\n%s\n", *data);
    }

    return recv_bytes + 1;
}

int tcp_close(struct tcp_connection *tcp_conn)
{
    close(tcp_conn->tcp_socket);

    return 1;
}


//HTTP

int http_get(char *host, char *url, void **data)
{
    struct tcp_connection tcp_conn;
    
    if(!tcp_connect(&tcp_conn, host, 80))
        return 0;

#define REQ_BUF_S 30
    int request_size = sizeof(host) + sizeof(url) + 30 + REQ_BUF_S;
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "Get %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);

    if(!tcp_send(&tcp_conn, request))
        return 0;

    int recv_bytes = tcp_recv(&tcp_conn, data);

    tcp_close(&tcp_conn); 

    return recv_bytes + 1;
}

int http_post(char *host, char *url, char *query_string, void **data)
{
    struct tcp_connection tcp_conn;

    if(!tcp_connect(&tcp_conn, host, 80))
            return 0;

    int q_string_s = sizeof(query_string);
    int request_size = sizeof(url) + q_string_s + int_s_length(q_string_s) + 70;  
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "POST %s HTTP/1.1\r\nContent-Type: text/plain\r\nContent_Length: %i\r\n\r\n%s", url, q_string_s, query_string);

    if(!tcp_send(&tcp_conn, request))
        return 0;

    int recv_bytes = tcp_recv(&tcp_conn, data);

    tcp_close(&tcp_conn);

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

int int_s_length(int val)
{
    int ret = 0;
    for(int i = 10000000000; i > 0; i /= 10)
    {
        if((val/i) >= 1)
            ret++;

        val %= i;
    }

    return ret;
}

#endif
