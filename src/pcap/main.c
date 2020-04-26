#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>

void packet_handler(u_char *arg, const struct pcap_pkthdr *hdr, const u_char *pkt);

int 
main() 
{
    char *dev;
    char err_buf[PCAP_ERRBUF_SIZE];
    pcap_t *h;
    int timeout = 10000;

    dev = pcap_lookupdev(err_buf);
    if (dev == NULL) {
        printf("Unable to find device: %s\n", err_buf);
        return 0;
    }

    h = pcap_open_live(dev, BUFSIZ, 0, timeout, err_buf);
    if (h == NULL) {
        printf("Could not open device %s: %s\n", dev, err_buf);
        return 0;
    }

    pcap_loop(h, 0, packet_handler, NULL);

    return 0;
}

void 
packet_handler(u_char *arg, const struct pcap_pkthdr *hdr, const u_char *pkt) 
{
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) pkt;

    printf("Packet capture length: %d\n", hdr->caplen);
    printf("Packet total length: %d\n", hdr->len);
    
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        printf("IP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        printf("ARP\n");
    } else  if (ntohs(eth_header->ether_type) == ETHERTYPE_REVARP) {
        printf("Reverse ARP\n");
    }
}
