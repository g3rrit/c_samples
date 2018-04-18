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

FILE *outfile = 0;

int main()
{
    TCP_SSL_LOG = stdout;
    TCP_LOG = stdout;

    outfile = fopen("out.txt", "w");

    char *data;
    //http_get("www.web.de", "/", &data);

    //http_post("man7.org", "/linux/man-pages/man2/socket.2.html", "", &data);

    https_get("www.reddit.com", "/", &http_c);

    //
    /*struct tcp_ssl_connection tcp_ssl;
    tcp_ssl_connect(&tcp_ssl, "www.web.de", 443);
    tcp_ssl_close(&tcp_ssl);
    */
    //free(data);
    //
    fclose(outfile);

    return 0;
}

int http_c(char *data, int size)
{
    fprintf(outfile, "%s", data);
    return 0;
}
