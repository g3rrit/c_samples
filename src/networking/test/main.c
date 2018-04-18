#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

int main()
{
    BIO *bio;
    SSL *ssl;
    SSL_CTX *ctx;

    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());

    if(!ctx)
        printf("error with ctx\n");

    bio = BIO_new_ssl_connect(ctx);

    BIO_set_conn_hostname(bio, "www.reddit.com:443");

    if(BIO_do_connect(bio) <= 0)
        printf("failed to connect\n");

    char *request = "GET / HTTP/1.1\r\nHost: www.reddit.com\r\n\r\n";

    printf("request: %s\n", request);

    if(BIO_write(bio, request, strlen(request)) <= 0)
        printf("error sending request\n");


    char buffer[1024];

    int bytes = 0;
    do
    {
        bytes = 0;
        bytes = BIO_read(bio, buffer, 1023);

        buffer[bytes] = 0;
        printf("response:\n%s\n", buffer);

    } while(bytes > 0);


    BIO_free_all(bio);
    SSL_CTX_free(ctx);

    return 0;
}
