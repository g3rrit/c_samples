#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main()
{
    struct addrinfo hints, *infoptr;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;

    int result = getaddrinfo("www.google.com" , NULL, &hints, &infoptr);
    if(result)
    {
        printf("error retreiving addrinfo \nerror: %s\n", gai_strerror(result));
        return 0;
    }

    struct addrinfo *ptr;

    char ip[256];
    memset(ip, 0, 256);

    for(ptr = infoptr; ptr != 0; ptr = ptr->ai_next)
    {
        getnameinfo(ptr->ai_addr, ptr->ai_addrlen, ip, sizeof(ip), 0, 0, NI_NUMERICHOST);
        printf("host: %s\n", ip);

    }

    freeaddrinfo(infoptr);

    return 0; 
}
