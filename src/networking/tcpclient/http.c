//hdr
#ifndef HTTP_H
#define HTTP_H

//HTTP

//return length of data returned from request
int http_get(char *host, char *url, void **data);

int http_get_to_file(char *host, char *url, char *file_url);

int http_post(char *host, char *url, char *query_string, void **data);

//HTTPS

int https_get(char *host, char *url, void **data);

int https_get_to_file(char *host, char *url, char *file_url);

int https_post(char *host, char *url, char *query_string, void **data);

#endif

//src
#ifndef HTTP_C

#ifndef TCP_C
#define TCP_C
#include "tcp.c"
#undef TCP_C
#endif

#ifndef TCPSSL_C
#define TCPSSL_C
#include "tcpssl.c"
#undef TCPSSL_C
#endif

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

    int recv_bytes = tcp_recv_dynamic(&tcp_conn, data);

    tcp_close(&tcp_conn); 

    return recv_bytes + 1;
}

int http_get_to_file(char *host, char *url, char *file_url)
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

    int recv_bytes = tcp_recv_to_file(&tcp_conn, file_url);

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

    int recv_bytes = tcp_recv_dynamic(&tcp_conn, data);

    tcp_close(&tcp_conn);

    return recv_bytes + 1;
}


//HTTPS

int https_get(char *host, char *url, void **data)
{
    struct tcp_ssl_connection tcp_conn;
    
    if(!tcp_ssl_connect(&tcp_conn, host, 443))
        return 0;

#define REQ_BUF_S 30
    int request_size = sizeof(host) + sizeof(url) + 30 + REQ_BUF_S;
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "Get %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);

    if(!tcp_ssl_send(&tcp_conn, request))
        return 0;

    int recv_bytes = tcp_ssl_recv_dynamic(&tcp_conn, data);

    tcp_ssl_close(&tcp_conn); 

    return recv_bytes + 1;
}

int https_get_to_file(char *host, char *url, char *file_url)
{
    struct tcp_ssl_connection tcp_conn;
    
    if(!tcp_ssl_connect(&tcp_conn, host, 443))
        return 0;

#define REQ_BUF_S 30
    int request_size = sizeof(host) + sizeof(url) + 30 + REQ_BUF_S;
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "Get %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);

    if(!tcp_ssl_send(&tcp_conn, request))
        return 0;

    int recv_bytes = tcp_ssl_recv_to_file(&tcp_conn, file_url);

    tcp_ssl_close(&tcp_conn); 

    return recv_bytes + 1;
}

int https_post(char *host, char *url, char *query_string, void **data)
{
    struct tcp_ssl_connection tcp_conn;

    if(!tcp_ssl_connect(&tcp_conn, host, 443))
            return 0;

    int q_string_s = sizeof(query_string);
    int request_size = sizeof(url) + q_string_s + int_s_length(q_string_s) + 70;  
    char request[request_size];

    bzero(request, request_size);

    sprintf(request, "POST %s HTTP/1.1\r\nContent-Type: text/plain\r\nContent_Length: %i\r\n\r\n%s", url, q_string_s, query_string);

    if(!tcp_ssl_send(&tcp_conn, request))
        return 0;

    int recv_bytes = tcp_ssl_recv_dynamic(&tcp_conn, data);

    tcp_ssl_close(&tcp_conn);

    return recv_bytes + 1;
}


#endif
