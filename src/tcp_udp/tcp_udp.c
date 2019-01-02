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
#include <netinet/udp.h>
#include <linux/if_ether.h>
#include <ctype.h>
#include <sys/types.h>

#define MAX_DATA_SIZE 1024

#define UDP_HDR_SIZE 8

#define INET_HDR_LEN 5


unsigned int ip_packet_create(uint8_t *packet, struct in_addr src_addr, struct in_addr dst_addr, uint8_t protocol) {
    struct iphdr *iph = (struct iphdr*) packet;

    iph->version = 4;
    iph->ihl = INET_HDR_LEN;
    iph->tos = 0;
    iph->tot_len = htons(INET_HDR_LEN * 4 + data_size);
    iph->id = 0; // Filled in automatically
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = protocol;
    iph->check = 0; // Filled in automatically
    iph->saddr = src_addr.s_addr;
    iph->daddr = dst_addr.s_addr;

    return sizeof(struct iphdr);
}

unsigned int udp_packet_create(uint8_t *packet, unsigned int data_size) {
    struct iphdr *iphd = (struct iphdr*) packet;
    struct udphdr *udph = (struct udphdr*) packet + sizeof(struct iphdr);

    length = UDP_HDR_SIZE + data_size;
    udph->source = iph->saddr.sin_port;
    udph->dest = iph->daddr.sin_port;
    udph->len = htons(length);
    udph->check = 0;

    return sizeof(struct udphdr);
}

unsigned int tcp_packet_create(uint8_t *packet, unsigned short src_port, unsigned short dest_port) {
  struct iphdr *iph = (struct iphdr*) packet;
  struct tcphdr *tcph = (struct tcphdr*) packet + sizeof(struct iphdr);

  tcph->th_sport = htons (src_port);	/* arbitrary port */
  tcph->th_dport = htons (dest_port);
  tcph->th_seq = random ();/* in a SYN packet, the sequence is a random */
  tcph->th_ack = 0;/* number, and the ack sequence is 0 in the 1st packet */
  tcph->th_x2 = 0;
  tcph->th_off = 0;		/* first and only tcp segment */
  tcph->th_flags = TH_SYN;	/* initial connection request */
  tcph->th_win = htonl (65535);	/* maximum allowed window size */
  tcph->th_sum = 0;/* if you set a checksum to zero, your kernel's IP stack
                      should fill in the correct checksum during transmission */
  tcph->th_urp = 0;

  return sizeof(struct tcphdr);
}

int send_packet(int s, struct sockaddr_in addr, uint8_t *packet, unsigned int packet_size) {
    if(sendto(s, packet, packet_size, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("sendto");
        return 0;
    }

    return 1;
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
    int s;
    uint8_t buffer[2048];
    unsigned int buffer_size = 0;
    unsigned int packet_size = 0;
    unsigned int data_size = 0;
    char *desthost = "127.0.0.1";
    char *srchost = "192.168.0.1";
    unsigned short src_port = 2028;
    unsigned short dest_port = 4086;
    struct sockaddr_in src_addr;
    struct sockaddr_in dst_addr;

    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(src_port);
    inet_aton(srchost, &src_addr.sin_addr);

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(dest_port);
    inet_aton(desthost, &dst_addr.sin_addr);

    printf("[+] Build IP packet...\n\n");
    packet_size = build_ip_packet(buffer, src_addr, dest_addr, IPPROTO_UDP);
    hexdump(buffer, packet_size);
    printf("\n\n");

    printf("[+] Build UDP packet...\n\n");
    packet_size = build_udp_packet(buffer, data_size);
    hexdump(udp_packet, packet_size);
    printf("\n\n");

    printf("[+] Send UDP packet...\n");
    if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
        perror("socket");
        exit(1);
    }
    send_packet(s, dst_addr, buffer, buffer_size);

    return 0;
}

