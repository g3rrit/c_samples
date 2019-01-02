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

unsigned short csum(unsigned short *buf, int nwords) {
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}


unsigned int ip_packet_create(uint8_t *packet, struct in_addr src_addr, struct in_addr dst_addr, uint8_t protocol) {
    struct iphdr *ip_header = (struct iphdr*) packet;

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

    return sizeof(struct iphdr);
}


#define MAX_PSEUDO_PKT_SIZE 1024

unsigned int udp_packet_create(uint8_t *packet, struct sockaddr_in src_addr, struct sockaddr_in dst_addr, uint8_t *data, unsigned int data_size) {
    uint8_t pseudo_packet[MAX_PSEUDO_PKT_SIZE];
    uint16_t length;
    struct udphdr *udph = (struct udphdr*) packet;
    struct pseudo_iphdr *p_iphdr = (struct pseudo_iphdr *)pseudo_packet;

    length = UDP_HDR_SIZE + data_size;
    udph->source = src_addr.sin_port;
    udph->dest = dst_addr.sin_port;
    udph->len = htons(length);

    if(length + sizeof(struct pseudo_iphdr) > MAX_PSEUDO_PKT_SIZE){
        fprintf(stderr, "Buffer size not enough");
        exit(1);
    }

    // Calculate checksum with pseudo ip header
    p_iphdr->source_addr = src_addr.sin_addr.s_addr;
    p_iphdr->dest_addr = dst_addr.sin_addr.s_addr;
    p_iphdr->zeros = 0;
    p_iphdr->prot = IPPROTO_UDP; //udp
    p_iphdr->length = udph->len;

    // Do NOT use udph->len instead of length.
    // udph->len is in big endian
    memcpy(pseudo_packet + sizeof(struct pseudo_iphdr), udph, length);
    memcpy(pseudo_packet + sizeof(struct pseudo_iphdr) + sizeof(struct udph), data_size);
    udph->check = cm(pseudo_packet, sizeof(struct pseudo_iphdr) + length);

    return sizeof(struct udphdr);
}

unsigned int tcp_packet_create(uint8_t *packet, struct sockaddr_in src_addr, struct sockaddr_in dest_addr, uint8_t *data, unsigned int data_size) {

  struct tcphdr *tcph = (struct tcphdr*) packet;

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

 
}

int send_tcp_packet(int s, struct sockaddr_in addr) {

  return 1;
}

int send_udp_packet(int s, struct sockaddr_in addr) {
    int flag = 1;
    unsigned int packet_size;
    unsigned int ip_payload_size;
    uint8_t packet[ETH_DATA_LEN];

    memset(packet, 0, ETH_DATA_LEN);
    ip_payload_size = build_udp_packet(src_addr, dst_addr, packet + sizeof(struct iphdr), data, data_size);

    packet_size = build_ip_packet(src_addr.sin_addr, dst_addr.sin_addr, IPPROTO_UDP, packet, packet + sizeof(struct iphdr), ip_payload_size);

    if(sendto(s, packet, packet_size, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr)) < 0){
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
    int raw_sock;
    uint8_t packet[ETH_DATA_LEN];
    uint8_t udp_packet[ETH_DATA_LEN];
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

    printf("[+] Build UDP packet...\n\n");
    packet_size = build_udp_packet(src_addr, dst_addr, udp_packet, data, data_size);
    hexdump(udp_packet, packet_size);
    printf("\n\n");

    printf("[+] Build IP packet...\n\n");
    packet_size = build_ip_packet(src_addr.sin_addr, dst_addr.sin_addr, IPPROTO_UDP, packet, udp_packet, packet_size);
    hexdump(packet, packet_size);
    printf("\n\n");

    printf("[+] Send UDP packet...\n");
    if((raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
        perror("socket");
        exit(1);
    }
    send_udp_packet(raw_sock, src_addr, dst_addr, data, data_size);

    return 0;
}

