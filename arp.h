#ifndef __ARP_UTIL_H__
#define __ARP_UTIL_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <unistd.h>
#include <getopt.h>

struct arp_packet
{
	struct ether_header eth_hdr;
	struct ether_arp arp;
};

void print_usage();


void set_hard_type(struct ether_arp *packet, unsigned short int type);
void set_prot_type(struct ether_arp *packet, unsigned short int type);
void set_hard_size(struct ether_arp *packet, unsigned char size);
void set_prot_size(struct ether_arp *packet, unsigned char size);
void set_op_code(struct ether_arp *packet, short int code);

void set_sender_hardware_addr(struct ether_arp *packet, char *address);
void set_sender_protocol_addr(struct ether_arp *packet, char *address);
void set_target_hardware_addr(struct ether_arp *packet, char *address);
void set_target_protocol_addr(struct ether_arp *packet, char *address);

char* get_target_protocol_addr(struct ether_arp *packet); 
char* get_sender_protocol_addr(struct ether_arp *packet); 
char* get_sender_hardware_addr(struct ether_arp *packet); 
char* get_target_hardware_addr(struct ether_arp *packet); 

char* get_locol_IP();
char* get_local_MAC();

void printHelp();
#endif
