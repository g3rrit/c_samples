//hdr
#ifndef TCPSSL_H
#define TCPSSL_H

#include <netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

struct tcp_ssl_connection
{
    char *dest;

    BIO *bio; 
    SSL *ssl;
    SSL_CTX *ctx;
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

int SSL_LIB_STARTED = 0;


FILE *TCP_SSL_LOG = 0;

int tcp_ssl_connect(struct tcp_ssl_connection *tcp_conn, char *host, int port)
{
    //initialize openssl
    if(!SSL_LIB_STARTED)
    {
        printf("starting ssl lib\n");
        SSL_library_init();
        SSL_LIB_STARTED = 1;
    }

    tcp_conn->ctx = SSL_CTX_new(SSLv23_client_method());

    if(!tcp_conn->ctx)
    {
        printf("ctx is null\n");
        return 0;
    }

    char ports[12];
    memset(ports, 12, 0);
    sprintf(ports, "%i", port);
    printf("ports: %s\n", ports);

    tcp_conn->dest = malloc(strlen(host) + strlen(ports) + 2);

    //create dest string www.website.de:443
    int offset = 0;
    strcpy(tcp_conn->dest, host);
    char dp = ':';
    offset += strlen(host);
    strcpy(tcp_conn->dest + offset, &dp);
    offset++;
    strcpy(tcp_conn->dest + offset, ports);
    offset += strlen(ports);
    tcp_conn->dest[offset] = 0;
    //-----
    
    printf("dest: %s\n", tcp_conn->dest);

    tcp_conn->bio = BIO_new_ssl_connect(tcp_conn->ctx);

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
        int bytes = BIO_write(tcp_conn->bio, request + sent_bytes, total_bytes - sent_bytes);
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
    int total_b = 0;
    int buffer_size = 1024;
    char r_buffer[buffer_size];
    do
    {
        bytes = 0;
        bytes = BIO_read(tcp_conn->bio, r_buffer, buffer_size - 1);

        printf("bytes: %i\n", bytes);

        total_b += bytes; 

        if(bytes > 0)
        {
            r_buffer[bytes] = 0;
            if(callback(r_buffer, bytes) == -1)
                break;
        }

    } while(bytes > 0);

    return total_b;
}

int tcp_ssl_close(struct tcp_ssl_connection *tcp_conn)
{
    BIO_free_all(tcp_conn->bio);

    SSL_CTX_free(tcp_conn->ctx);

    free(tcp_conn->dest);

    return 1;
}

#endif
