#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#ifndef __USE_BSD
#define __USE_BSD
#endif
#include <netinet/ip.h>
#ifndef __FAVOR_BSD
#define __FAVOR_BSD
#endif
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#ifndef KERNEL
#define KERNEL
#endif
#ifndef __APPLE_API_PRIVATE
#define __APPLE_API_PRIVATE
#endif
#include <netinet/in.h>
#include <net/if.h>

#define PORT 80

unsigned short csum(unsigned short *ptr,int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;

    return(answer);
}

struct packet_t {
  char *src_ip;
  int src_port;

  char *dest_ip;
  int dest_port;
};

int send_rpacket(struct packet_t *packet)
{
  int i;
  int raw_sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);

  char datagram[4096];
  char cksum_tcpheader[1024];

  struct ip *iph = (struct ip *) &datagram;
  struct tcphdr *tcph = (struct tcphdr *) &datagram+1;
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (packet->dest_port);
  sin.sin_addr.s_addr = inet_addr (packet->dest_ip);

  memset(datagram, 0x0, sizeof(datagram));
  memset(cksum_tcpheader, 0, sizeof(cksum_tcpheader));

  //IP header
  iph->ip_hl = 5;
  iph->ip_v = IPVERSION;
  iph->ip_tos = IPTOS_PREC_ROUTINE;
  iph->ip_len = htons(sizeof (struct ip) + sizeof (struct tcphdr));
  iph->ip_id = htonl(11111);
  iph->ip_off = 0x0;
  iph->ip_ttl = MAXTTL;
  iph->ip_p = 6;
  iph->ip_sum = 0x0;
  iph->ip_src.s_addr = inet_addr (packet->src_ip);
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;

  //TCP Header
  tcph->th_sport = htons(packet->src_port);
  tcph->th_dport = htons(packet->dest_port);
  tcph->th_seq = random();
  tcph->th_ack = 0x0;
  tcph->th_x2 = 0x0;
  tcph->th_off = sizeof(struct tcphdr) + 1;
  tcph->th_flags = TH_SYN;
  tcph->th_win = htonl(TCP_MAXWIN);
  tcph->th_sum = 0x0;
  tcph->th_urp = 0x0;

  iph->ip_sum = csum ((unsigned short *) iph, sizeof(struct ip));

  //special treatment for tcp header checksum 0
  memcpy(&cksum_tcpheader, &(iph->ip_src.s_addr), 4);
  memcpy(&cksum_tcpheader[4], &(iph->ip_dst.s_addr), 4);
  cksum_tcpheader[8]=0;
  cksum_tcpheader[9]=(u_int16_t)iph->ip_p;
  cksum_tcpheader[10]=(u_int16_t) (sizeof(struct tcphdr) & 0xff00) >> 8;
  cksum_tcpheader[11]=(u_int16_t) (sizeof(struct tcphdr) & 0x00ff);
  memcpy(&cksum_tcpheader[12], tcph, sizeof(struct tcphdr));

  tcph->th_sum = csum((unsigned short *) (cksum_tcpheader), sizeof(struct tcphdr)+12);

  //set socket option to include the header
  int one = 1;
  const int *val = &one;
  setsockopt (raw_sock, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));

  sendto(raw_sock, datagram, iph->ip_len, SO_KEEPALIVE, (struct sockaddr *) &sin, sizeof (const int *));

  return 0;
}

int main(void) {

  struct packet_t packet = {
    .src_ip = "127.0.0.1",
    .src_port = 30,

    .dest_ip = "123.123.123.123",
    .dest_port = 80
  };

  send_rpacket(&packet);

  return 0;
}
