//hdr
#ifndef TCPSSL_H
#define TCPSSL_H

#include <netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

struct tcp_ssl_connection
{
    int port;
    char ports[12];

    char *dest;

    BIO *bio; 
    SSL *ssl_conn;
    SSL_CTX *ssl_ctx;
};

int tcp_ssl_connect(struct tcp_ssl_connection *tcp_conn, char *host, int port);

int tcp_ssl_send(struct tcp_ssl_connection *tcp_conn, char *request);

int tcp_ssl_recv(struct tcp_ssl_connection *tcp_conn, int (*callback)(char *data, int size));

int tcp_ssl_close(struct tcp_ssl_connection *tcp_conn);

extern FILE *TCP_SSL_LOG;

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

    if(!tcp_conn->ssl_ctx)
    {
        printf("ctx is null\n");
        return 0;
    }

    tcp_conn->port = port;

    memset(tcp_conn->ports, 12, 0);
    sprintf(tcp_conn->ports, "%i", port);
    printf("ports: %s\n", tcp_conn->ports);

    //create ssl connection and attack it to the socket
    //tcp_conn->ssl_conn = SSL_new(tcp_conn->ssl_ctx); 
    //SSL_set_fd(tcp_conn->ssl_conn, tcp_conn->tcp_socket);
    //
    /*
    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "+ tcp socket succesfully opened\n");

    if((tcp_conn->server = gethostbyname(host)) == 0)
    {
        close(tcp_conn->tcp_socket);
        printf("-----!error getting hostname!-----\n");
        return 0; 
    }

    */

    /*j
    if(TCP_SSL_LOG)
        fprintf(TCP_SSL_LOG, "%s -----------\n", tcp_conn->server->h_name);
        */
    
    /*if(TCP_SSL_LOG)
    {
        for(int i = 0; tcp_conn->server->h_addr_list[i]; i++)
            fprintf(TCP_SSL_LOG, " -> %s\n", inet_ntoa(*(struct in_addr*)(tcp_conn->server->h_addr_list[i]))); 
    }
    */

    /*
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
        */

    tcp_conn->bio = BIO_new_ssl_connect(tcp_conn->ssl_ctx);

    tcp_conn->dest = malloc(strlen(host) + strlen(tcp_conn->ports) + 2);

    int offset = 0;

    strcpy(tcp_conn->dest, host);
    char dp = ':';
    offset += strlen(host);
    strcpy(tcp_conn->dest + offset, &dp);
    offset++;
    strcpy(tcp_conn->dest + offset, tcp_conn->ports);
    offset += strlen(tcp_conn->ports);
    tcp_conn->dest[offset] = 0;
    
    printf("dest: %s\n", tcp_conn->dest);

    BIO_set_conn_hostname(tcp_conn->bio, tcp_conn->dest);

    if(BIO_do_connect(tcp_conn->bio) <= 0)
    {
        printf("failed to connect\n");
        return 0;
    }

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
        int bytes = BIO_write(tcp_conn->ssl_conn, request + sent_bytes, total_bytes - sent_bytes);
        printf("total: %i\n", total_bytes);
        printf("bytes: %i\n", bytes);
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

int tcp_ssl_recv(struct tcp_ssl_connection *tcp_conn, int (*callback)(char *data, int size))
{
    int bytes = 0;
    char buffer_size = 1024;
    char buffer[buffer_size];
    do
    {
        bytes = BIO_read(tcp_conn->ssl_conn, buffer, buffer_size);

        printf("bytes: %i\n", bytes);

        if(bytes > 0)
        {
            buffer[bytes] = 0;
            if(callback(buffer, bytes) == -1)
                break;
        }

    } while(bytes > 0);
}

int tcp_ssl_close(struct tcp_ssl_connection *tcp_conn)
{
    /*
    SSL_shutdown(tcp_conn->ssl_conn);
    SSL_free(tcp_conn->ssl_conn);

    close(tcp_conn->tcp_socket);
    */

    BIO_free_all(tcp_conn->bio);

    SSL_CTX_free(tcp_conn->ssl_ctx);

    free(tcp_conn->dest);

    return 1;
}

#endif
