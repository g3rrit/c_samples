#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> 
#include <linux/if_ether.h>
#include <ctype.h>
#include <sys/types.h>

#define MAX_DATA_SIZE 1024

#define ICMP_HDR_SIZE 8

#define INET_HDR_LEN 5


// For little endian
struct pseudo_iphdr
{
    uint32_t source_addr;
    uint32_t dest_addr;
    uint8_t zeros;
    uint8_t prot;
    uint16_t length;
};


uint16_t checksum(uint8_t *data, unsigned int size)
{
    int i;
    int sum = 0;
    uint16_t *p = (uint16_t *)data;

    for(i = 0; i < size; i += 2){
        sum += *(p++);
    }

    uint16_t carry = sum >> 16;
    uint16_t tmp = 0x0000ffff & sum;
    uint16_t res = ~(tmp + carry);

    return res;
}


unsigned int build_ip_packet(struct in_addr src_addr, struct in_addr dst_addr, uint8_t protocol, uint8_t *ip_packet, uint8_t *data, unsigned int data_size)
{
    struct iphdr *ip_header;

    ip_header = (struct iphdr *)ip_packet;
    ip_header->version = 4;
    ip_header->ihl = INET_HDR_LEN;
    ip_header->tos = 0;
    ip_header->tot_len = htons(INET_HDR_LEN * 4 + data_size);
    ip_header->id = 0; // Filled in automatically
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = protocol;
    ip_header->check = 0; // Filled in automatically
    ip_header->saddr = src_addr.s_addr;
    ip_header->daddr = dst_addr.s_addr;

    memcpy(ip_packet + sizeof(struct iphdr), data, data_size);

    return sizeof(struct iphdr) + data_size;
}


unsigned int build_icmp_packet(struct sockaddr_in src_addr, struct sockaddr_in dst_addr, uint8_t *icmp_packet, uint8_t *data, unsigned int data_size)
{
    uint16_t length;
    struct icmphdr *icmph;

    length = ICMP_HDR_SIZE + data_size;
    icmph = (struct icmphdr *)icmp_packet;
    icmph->type = ICMP_ECHO;
    icmph->un.echo.id = getpid(); 
	  icmph->un.echo.sequence = 0; 

    // check if data fits in package

    memcpy(icmp_packet + ICMP_HDR_SIZE, data, data_size);
	  icmph->checksum = checksum(icmp_packet, length); 

    return length;
}


void send_icmp_packet(int raw_sock, struct sockaddr_in src_addr, struct sockaddr_in dst_addr, uint8_t *data, unsigned int data_size)
{
    int flag = 1;
    unsigned int packet_size;
    unsigned int ip_payload_size;
    uint8_t packet[ETH_DATA_LEN];

    memset(packet, 0, ETH_DATA_LEN);
    ip_payload_size = build_icmp_packet(src_addr, dst_addr, packet + sizeof(struct iphdr), data, data_size);

    packet_size = build_ip_packet(src_addr.sin_addr, dst_addr.sin_addr, IPPROTO_UDP, packet, packet + sizeof(struct iphdr), ip_payload_size);

    if(sendto(raw_sock, packet, packet_size, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr)) < 0){
        perror("sendto");
        exit(1);
    }
}


void hexdump(unsigned char *data, unsigned int data_bytes)
{
    int bin_p, ascii_p;

    bin_p = ascii_p = 0;

    while(bin_p < data_bytes){
        int j;
        int whitespaces;
        for(j = 0; j < 8 && bin_p < data_bytes; j++){
            printf("%02x ", data[bin_p++]);
        }

        whitespaces = (8 - j) * 3;
        for(j = 0; j < whitespaces; j++){
            printf(" ");
        }

        for(j = 0; j < 8 && ascii_p < data_bytes; j++){
            if(isprint(data[ascii_p])){
                printf("%c", data[ascii_p++]);
            }else{
                printf(".");
                ascii_p++;
            }
        }

        printf("\n");
    }
}


int main(void)
{
    int raw_sock;
    uint8_t packet[ETH_DATA_LEN];
    uint8_t icmp_packet[ETH_DATA_LEN];
    uint8_t data[MAX_DATA_SIZE];
    char *sending_data = "AAAAAAAA";
    char *localhost = "127.0.0.1";
    char *srchost = "192.168.0.1";
    unsigned int packet_size;
    unsigned int data_size;
    struct sockaddr_in src_addr;
    struct sockaddr_in dst_addr;

    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(2048);
    inet_aton(srchost, &src_addr.sin_addr);

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(4086);
    inet_aton(localhost, &dst_addr.sin_addr);

    strcpy((char *)data, sending_data);
    data_size = strlen(sending_data);

    printf("[+] Build ICMP packet...\n\n");
    packet_size = build_icmp_packet(src_addr, dst_addr, icmp_packet, data, data_size);
    hexdump(icmp_packet, packet_size);
    printf("\n\n");

    printf("[+] Build IP packet...\n\n");
    packet_size = build_ip_packet(src_addr.sin_addr, dst_addr.sin_addr, IPPROTO_UDP, packet, icmp_packet, packet_size);
    hexdump(packet, packet_size);
    printf("\n\n");

    printf("[+] Send ICMP packet...\n");
    if((raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
        perror("socket");
        exit(1);
    }
    send_icmp_packet(raw_sock, src_addr, dst_addr, data, data_size);

    return 0;
}
