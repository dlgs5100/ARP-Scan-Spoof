
#include "arp.h"

/* 
 * Change "enp2s0f5" to your device name (e.g. "eth0"), when you test your hoework.
 * If you don't know your device name, you can use "ifconfig" command on Linux.
 * You have to use "enp2s0f5" when you ready to upload your homework.
 */
#define DEVICE_NAME "enp0s3"

/*
 * You have to open two socket to handle this program.
 * One for input , the other for output.
 */
struct option long_options[] = {
	{"help", 0, NULL, 'h'},
	{"query", 1, NULL, 'q'},
	{"list", 1, NULL, 'l'},
	{0, 0, 0, 0},
};
char *l_opt_arg;
char* const short_options = "hq:l:";

void printHelp();
int main(int argc, char* argv[]){
	int sockfd_recv = 0, sockfd_send = 0;
	struct sockaddr sa;
	struct sockaddr_ll saddr_ll;
	struct ifreq ifr;
	// struct in_addr myip;
	int recvlen, sendlen;
	//u_int8_t arrARP[1500];
	char sender_addr[16], target_addr[16], target_MAC[18];
	struct arp_packet rcv_pak;
	
	if(geteuid() != 0)
		printf("ERROR: You must be root to use this tool!\n");
	else{
		char c;
		while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1){
			printf("[ ARP sniffer and spoof program ]\n");
			switch(c){
				case'h':
					printHelp();
					break;
				case'l':
					printf("### ARP sniffer mode ###\n");
					if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
					{
						perror("open recv socket error");
						exit(1);
					}
					// struct arp_packet rcv_pak;
					if(strcmp(optarg, "-a") == 0){
						while(1){
							if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0)
							{	
								perror("recvfrom");
								exit(1);
							}
							//?
							strcpy(target_addr, get_target_protocol_addr(&(rcv_pak.arp)));	
							strcpy(sender_addr, get_sender_protocol_addr(&(rcv_pak.arp)));
							printf("Get ARP packet - Who has %s ?\t Tell %s\n", sender_addr, target_addr);		
						}
					}
					else{
						while(1){
							if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0)
							{	
								perror("recvfrom");
								exit(1);
							}

							strcpy(target_addr, get_target_protocol_addr(&(rcv_pak.arp)));	
							strcpy(sender_addr, get_sender_protocol_addr(&(rcv_pak.arp)));
							if(strcmp(optarg, sender_addr) == 0)
								printf("Get ARP packet - Who has %s ?\t Tell %s\n", sender_addr, target_addr);		
						}
					}
					break;
				case'q':
					printf("### ARP query mode ###\n");

					if((sockfd_send = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_RARP))) < 0)
					{
							perror("socket error");
							exit(1);
					}
					struct arp_packet *snd_pak = (struct arp_packet *)malloc(sizeof(struct arp_packet));

					set_ether_dst_addr(&(snd_pak->eth_hdr), "52:54:00:12:35:02");
					set_ether_src_addr(&(snd_pak->eth_hdr), get_local_MAC());
					//printf("ABD: %d", htons(ETHERTYPE_ARP));
					snd_pak->eth_hdr.ether_type = htons(ETHERTYPE_ARP);

					set_hard_type(&(snd_pak->arp), htons(ARPHRD_ETHER));
					set_prot_type(&(snd_pak->arp), htons(ETHERTYPE_IP));
					set_hard_size(&(snd_pak->arp), 6);
					set_prot_size(&(snd_pak->arp), 4);
					set_op_code(&(snd_pak->arp), htons(1));

					set_sender_hardware_addr(&(snd_pak->arp), "\0");
					set_sender_protocol_addr(&(snd_pak->arp), optarg);
					set_target_hardware_addr(&(snd_pak->arp), get_local_MAC());
					set_target_protocol_addr(&(snd_pak->arp), get_locol_IP());
					
					bzero(&sa, sizeof(sa));
					sa.sa_family = PF_PACKET;
					strcpy(sa.sa_data, DEVICE_NAME);
					
					if((sendlen = sendto(sockfd_send, (void*)snd_pak, sizeof(struct arp_packet), 0, &sa, sizeof(struct sockaddr)))<0)
					{
						perror("sendto");
						exit(1);
					}

					

					// struct arp_packet rcv_pak;
					// if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
					// {
					// 	perror("open recv socket error");
					// 	exit(1);
					// }
					// if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0)
					// {	
					// 	perror("recvfrom");
					// 	exit(1);
					// }
					// //?
					// strcpy(target_addr, get_target_protocol_addr(&(rcv_pak.arp)));	
					// strcpy(target_MAC, get_target_hardware_addr(&(rcv_pak.arp)));
					// printf("MAC address of %s is %s\n", target_addr, target_MAC);
					// fflush(stdout);
					break;
				default:
					printf("Unexist option\n");

			}
		}
	}
	
	// Open a send socket in data-link layer.
	if((sockfd_send = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		perror("open send socket error");
		exit(sockfd_send);
	}
	
	/*
	 * Use ioctl function binds the send socket and the Network Interface Card.
`	 * ioctl( ... )
	 */
	
	

	
	// Fill the parameters of the sa.



	
	/*
	 * use sendto function with sa variable to send your packet out
	 * sendto( ... )
	 */
	
	
	


	return 0;
}