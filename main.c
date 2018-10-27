
#include "arp.h"

/* 
 * Change "enp2s0f5" to your device name (e.g. "eth0"), when you test your hoework.
 * If you don't know your device name, you can use "ifconfig" command on Linux.
 * You have to use "enp2s0f5" when you ready to upload your homework.
 */
#define DEVICE_NAME "enp2s0f5"

/*
 * You have to open two socket to handle this program.
 * One for input , the other for output.
 */

void printHelp();
int main(int argc, char* argv[])
{
	int sockfd_recv = 0, sockfd_send = 0;
	// struct sockaddr_ll sa;
	// struct ifreq req;
	// struct in_addr myip;
	int recvlen;
	struct arp_packet pak;
	//u_int8_t arrARP[1500];
	char target_addr[16];
	
	if(geteuid() != 0)
		printf("ERROR: You must be root to use this tool!\n");
	else{
		char c;
		while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1){
			switch(c){
				case'h':
					printHelp();
					break;
				case'l':
					if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
					{
						perror("open recv socket error");
						exit(1);
					}

					if(strcmp(optarg, "-a") == 0){
						while(1){
							if((recvlen = recvfrom( sockfd_recv, (void *)&pak, sizeof(struct arp_packet), 0, NULL, NULL))<0)//change arp_packet
							{	
								perror("recvfrom");
								exit(1);
							}
							//memcpy(arrARP, (void *)&pak, sizeof(struct arp_packet));//copy struct arp_packet into arp_packet array

							// struct in_addr addr1;
							// memcpy(&addr1, &pak.arp.arp_tpa, 4);
							// printf("%s\n", inet_ntoa(addr1));
							
							//strcpy(sender_addr, get_sender_protocol_addr(&(pak.arp)));
							strcpy(target_addr, get_target_protocol_addr(&(pak.arp)));	
							printf("%s\n", target_addr);		
							
						}
					}
					else{

					}
					break;
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

void printHelp(){
	printf("[ ARF sniffer and spoof program]\n");
	printf("Format :\n");
	printf("1) ./arp -l -a\n");
	printf("2) ./arp -l <filter_ip_address>\n");
	printf("3) ./arp -q <query_ip_address>\n");
	printf("4) ./arp <fake_mac_address> <target_ip_address>\n");
}