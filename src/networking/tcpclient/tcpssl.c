//hdr
#ifndef TCPSSL_H
#define TCPSSL_H

#include <netinet/in.h>

#include <openssl/ssl.h>

struct tcp_ssl_connection
{
    struct hostent *server;
    struct sockaddr_in server_addr;
    int port;
    int tcp_socket;

    SSL *ssl_conn;
    SSL_CTX *ssl_ctx;
};

int tcp_ssl_connect(struct tcp_ssl_connection *tcp_conn, char *host, int port);

int tcp_ssl_send(struct tcp_ssl_connection *tcp_conn, char *request);

int tcp_ssl_recv_dynamic(struct tcp_ssl_connection *tcp_conn, char **data);

int tcp_ssl_recv_static(struct tcp_ssl_connection *tcp_conn, char *data, int size);

int tcp_ssl_recv_to_file(struct tcp_ssl_connection *tcp_conn, char *url);

int tcp_ssl_close(struct tcp_ssl_connection *tcp_conn);

#endif

//src
#ifndef TCPSSL_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#ifndef TCPUTIL_C
#define TCPUTIL_C
#include "tcputil.c"
#undef TCPUTIL_C
#endif


FILE *TCP_SSL_LOG = 0;

int tcp_ssl_connect(struct tcp_ssl_connection *tcp_conn, char *host, int port)
{
    //initialize openssl
    SSL_load_error_strings();
    SSL_library_init();
    tcp_conn->ssl_ctx = SSL_CTX_new(SSLv23_client_method());
    //

    tcp_conn->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    tcp_conn->port = port;

    if(tcp_conn->tcp_socket == -1)
    {
        printf("-----!error opening tcp_socket!-----\n");
        return 0;
    }

    //create ssl connection and attack it to the socket
    tcp_conn->ssl_conn = SSL_new(tcp_conn->ssl_ctx); 
    SSL_set_fd(tcp_conn->ssl_conn, tcp_conn->tcp_socket);

    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "+ tcp socket succesfully opened\n");

    if((tcp_conn->server = gethostbyname(host)) == 0)
    {
        close(tcp_conn->tcp_socket);
        printf("-----!error getting hostname!-----\n");
        return 0; 
    }

    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "%s -----------\n", tcp_conn->server->h_name);
    
    /*if(TCP_SSL_LOG)
    {
        for(int i = 0; tcp_conn->server->h_addr_list[i]; i++)
            fprintf(TCP_SSL_LOG, " -> %s\n", inet_ntoa(*(struct in_addr*)(tcp_conn->server->h_addr_list[i]))); 
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

    int err = SSL_connect(tcp_conn->ssl_conn);

    if(err != 1)
    {
        printf("-----!error performing ssl handshake!-----\n");
        printf("-----!error: %i\n", err);
        return 0;
    }

    
    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "succefully connected to server\n");

    return 1;
}

int tcp_ssl_send(struct tcp_ssl_connection *tcp_conn, char *request)
{
    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "request:\n%s\n", request);

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

    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "succesfully sent request\n");

    return total_bytes;
}

int tcp_ssl_recv_dynamic(struct tcp_ssl_connection *tcp_conn, char **data)
{
    //receive response
#define UPPER_BUFF_S 32768
    int recv_bytes = 0;
    int buffer_size = 4096;
    *data = malloc(buffer_size);
    while(recv_bytes < UPPER_BUFF_S)
    {
        int bytes = SSL_read(tcp_conn->ssl_conn, *data + recv_bytes, buffer_size - recv_bytes);
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

    if(TCP_SSL_LOG)
    {
        fprintf(TCP_SSL_LOG, "received %i bytes\n", recv_bytes);
        fprintf(TCP_SSL_LOG, "response:\n%s\n", *data);
    }

    return recv_bytes + 1;
}

int tcp_ssl_recv_static(struct tcp_ssl_connection *tcp_conn, char *data, int size)
{
    int recv_bytes = 0;
    while(recv_bytes < size - 1)
    {
        int bytes = SSL_read(tcp_conn->ssl_conn, data + recv_bytes, size - recv_bytes);
        if(bytes < 0)
        {
            printf("-----!error receiving response!-----\n");
            return 0;
        }
        if(bytes == 0)
            break;
        recv_bytes += bytes;
    }

    data[recv_bytes] = 0;

    if(TCP_SSL_LOG)
    {
        fprintf(TCP_SSL_LOG, "received %i bytes\n", recv_bytes);
        fprintf(TCP_SSL_LOG, "response:\n%s\n", *data);
    }

    return recv_bytes + 1;
}

int tcp_ssl_recv_to_file(struct tcp_ssl_connection *tcp_conn, char *url)
{
    FILE *file = fopen(url, "wb");
    if(!file)
        printf("-----!error opening file!------\n");

    int recv_bytes = 0;
    int bytes = 1;
    unsigned char buffer[1024];
    int buffer_size = 1024;
    while(bytes != 0)
    {
        bytes = SSL_read(tcp_conn->ssl_conn, buffer, buffer_size);
        if(bytes < 0)
        {
            printf("-----!error receiving response!-----\n");
            return 0;
        }
        fwrite(buffer, bytes, 1, file);
        recv_bytes += bytes;
    }

    /*if(TCP_SSL_LOG)
    {
        fprintf(TCP_SSL_LOG, "received %i bytes\n", recv_bytes);
        fprintf(TCP_SSL_LOG, "response:\n%s\n", *data);
    }*/

    fclose(file);

    return recv_bytes + 1;
}

int tcp_ssl_close(struct tcp_ssl_connection *tcp_conn)
{
    SSL_shutdown(tcp_conn->ssl_conn);
    SSL_free(tcp_conn->ssl_conn);

    close(tcp_conn->tcp_socket);

    return 1;
}

#endif
