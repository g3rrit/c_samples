#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	/* these headers are for a Linux system, but */
#include <netinet/in.h>	/* the names on other systems are easy to guess.. */
#include <netinet/ip.h>
#include <arpa/inet.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned short csum(unsigned short *buf, int nwords) {
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int send_tcp_packet(char *src_ip, unsigned short src_port, char *dest_ip, unsigned short dest_port) {
  int s = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);	/* open raw socket */
  if(s < 0) {
    perror("socket");
    return 0;
  }
  char datagram[4096];	/* this buffer will contain ip header, tcp header,
                           and payload. we'll point an ip header structure
                           at its beginning, and a tcp header structure after
                           that to write the header values into it */
  struct ip *iph = (struct ip *) datagram;
  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);
  struct sockaddr_in sin_dest;
  struct sockaddr_in sin_src;

  /* the sockaddr_in containing the dest. address is used
     in sendto() to determine the datagrams path */

  sin_dest.sin_family = AF_INET;
  sin_dest.sin_port = htons (dest_port);/* you byte-order >1byte header values to network
                              byte order (not needed on big endian machines) */
  //sin.sin_addr.s_addr = inet_addr (dest_ip);
  inet_pton(PF_INET, src_ip, &(sin_src.sin_addr));
  inet_pton(PF_INET, dest_ip, &(sin_dest.sin_addr));

  memset (datagram, 0, 4096);	/* zero out the buffer */

  /* we'll now fill in the ip/tcp header values, see above for explanations */
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len = sizeof (struct ip) + sizeof (struct tcphdr);	/* no payload */
  iph->ip_id = htonl (54321);	/* the value doesn't matter here */
  iph->ip_off = 0;
  iph->ip_ttl = 255;
  iph->ip_p = 6;
  iph->ip_sum = 0;		/* set it to 0 before computing the actual checksum later */
  iph->ip_src.s_addr = sin_src.sin_addr.s_addr;
  iph->ip_dst.s_addr = sin_dest.sin_addr.s_addr;
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

  iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);

  /* finally, it is very advisable to do a IP_HDRINCL call, to make sure
     that the kernel knows the header is included in the data, and doesn't
     insert its own header into the packet before our data */

  {				/* lets do it the ugly way.. */
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");
  }

  if (sendto (s,		/* our socket */
        datagram,	/* the buffer containing headers and data */
        iph->ip_len,	/* total length of our datagram */
        0,		/* routing flags, normally always 0 */
        (struct sockaddr *) &sin_dest,	/* socket addr, just like in */
        sizeof (sin_dest)) < 0) {		/* a normal send() */ 
    perror("sending");
    return 0;
  }

  return 1;
}

int main(void) {
  send_tcp_packet("127.0.0.1", 80, "151.217.246.103", 8833);

  return 0;
}

