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

#ifndef HTTP_C
#define HTTP_C
#include "http.c"
#undef HTTP_C
#endif

#include <stdlib.h>

int http_c(char *data, int size);

int main()
{
    TCP_SSL_LOG = stdout;
    TCP_LOG = stdout;

    char *data;
    //http_get("www.web.de", "/", &data);

    //http_post("man7.org", "/linux/man-pages/man2/socket.2.html", "", &data);

    https_get("www.openssl.org", "/docs/man1.0.2/ssl/SSL_set_fd.html", &http_c);

    //
    /*struct tcp_ssl_connection tcp_ssl;
    tcp_ssl_connect(&tcp_ssl, "www.web.de", 443);
    tcp_ssl_close(&tcp_ssl);
    */
    //free(data);

    return 0;
}

int http_c(char *data, int size)
{
    printf("received:\n%s\n", data);
    return 0;
}
