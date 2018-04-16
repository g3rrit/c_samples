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

int main()
{
    TCP_LOG = stdout;

    void *data;
    //http_get("www.web.de", "/", &data);

    //http_post("man7.org", "/linux/man-pages/man2/socket.2.html", "", &data);

    https_get_to_file("www.man7.org", "/linux/man-pages/man2/socket.2.html", "out2.txt");
    //
    struct tcp_ssl_connection tcp_ssl;
    tcp_ssl_connect(&tcp_ssl, "www.web.de", 443);
    tcp_ssl_close(&tcp_ssl);

    free(data);

    return 0;
}
