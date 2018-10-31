
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
		printf("[ ARP sniffer and spoof program ]\n");
		if(!strcmp(argv[1], "-help")){
			printHelp();
		}
		else if(!strcmp(argv[1], "-l")){
			printf("### ARP sniffer mode ###\n");
			if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
				perror("open recv socket error");
				exit(1);
			}
			// struct arp_packet rcv_pak;
			if(!strcmp(argv[2], "-a")){
				while(1){
					if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0){	
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
					if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0){	
						perror("recvfrom");
						exit(1);
					}

					strcpy(target_addr, get_target_protocol_addr(&(rcv_pak.arp)));	
					strcpy(sender_addr, get_sender_protocol_addr(&(rcv_pak.arp)));
					if(strcmp(argv[2], sender_addr) == 0)
						printf("Get ARP packet - Who has %s ?\t Tell %s\n", sender_addr, target_addr);		
				}
			}
		}
		else if(!strcmp(argv[1], "-q")){
			printf("### ARP query mode ###\n");

			if((sockfd_send = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_RARP))) < 0){
				perror("socket error");
				exit(1);
			}
			struct arp_packet *snd_pak = (struct arp_packet *)malloc(sizeof(struct arp_packet));

			set_ether_dst_addr(&(snd_pak->eth_hdr), "ff:ff:ff:ff:ff:ff");
			set_ether_src_addr(&(snd_pak->eth_hdr), get_local_MAC());
			//printf("ABD: %d", htons(ETHERTYPE_ARP));
			snd_pak->eth_hdr.ether_type = htons(ETHERTYPE_ARP);

			set_hard_type(&(snd_pak->arp), htons(ARPHRD_ETHER));
			set_prot_type(&(snd_pak->arp), htons(ETHERTYPE_IP));
			set_hard_size(&(snd_pak->arp), 6);
			set_prot_size(&(snd_pak->arp), 4);
			set_op_code(&(snd_pak->arp), htons(1));

			set_sender_hardware_addr(&(snd_pak->arp), get_local_MAC());
			set_sender_protocol_addr(&(snd_pak->arp), get_locol_IP());
			set_target_hardware_addr(&(snd_pak->arp), "00:00:00:00:00:00");
			set_target_protocol_addr(&(snd_pak->arp), argv[2]);
					
			bzero(&sa, sizeof(sa));
			sa.sa_family = PF_PACKET;
			strcpy(sa.sa_data, DEVICE_NAME);
					
			if((sendlen = sendto(sockfd_send, (void*)snd_pak, sizeof(struct arp_packet), 0, &sa, sizeof(struct sockaddr)))<0){
				perror("sendto");
				exit(1);
			}

			struct arp_packet rcv_pak;
			if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
				perror("open recv socket error");
				exit(1);
			}
			if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0){	
				perror("recvfrom");
				exit(1);
			}
			//?
			strcpy(target_addr, get_sender_protocol_addr(&(rcv_pak.arp)));	
			strcpy(target_MAC, get_sender_hardware_addr(&(rcv_pak.arp)));
			printf("MAC address of %s is %s\n", target_addr, target_MAC);
			fflush(stdout);
		}
		else{
			printf("### ARP spoof mode ###\n");
			struct arp_packet rcv_pak;
			if((sockfd_recv = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
				perror("open recv socket error");
				exit(1);
			}
			if((recvlen = recvfrom( sockfd_recv, (void *)&rcv_pak, sizeof(struct arp_packet), 0, NULL, NULL))<0){	
				perror("recvfrom");
				exit(1);
			}
			strcpy(target_addr, get_target_protocol_addr(&(rcv_pak.arp)));	
			strcpy(sender_addr, get_sender_protocol_addr(&(rcv_pak.arp)));
			if(strcmp(argv[2], sender_addr) == 0)
				printf("Get ARP packet - Who has %s ?\t Tell %s\n", sender_addr, target_addr);

			if((sockfd_send = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_RARP))) < 0){
				perror("socket error");
				exit(1);
			}
			struct arp_packet *snd_pak = (struct arp_packet *)malloc(sizeof(struct arp_packet));

			set_ether_dst_addr(&(snd_pak->eth_hdr), get_local_MAC());
			set_ether_src_addr(&(snd_pak->eth_hdr), get_local_MAC());
			//printf("ABD: %d", htons(ETHERTYPE_ARP));
			snd_pak->eth_hdr.ether_type = htons(ETHERTYPE_ARP);

			set_hard_type(&(snd_pak->arp), htons(ARPHRD_ETHER));
			set_prot_type(&(snd_pak->arp), htons(ETHERTYPE_IP));
			set_hard_size(&(snd_pak->arp), 6);
			set_prot_size(&(snd_pak->arp), 4);
			set_op_code(&(snd_pak->arp), htons(2));

			set_sender_hardware_addr(&(snd_pak->arp), argv[1]);
			set_sender_protocol_addr(&(snd_pak->arp), argv[2]);
			set_target_hardware_addr(&(snd_pak->arp), get_local_MAC());
			set_target_protocol_addr(&(snd_pak->arp), get_locol_IP());
					
			bzero(&sa, sizeof(sa));
			sa.sa_family = PF_PACKET;
			strcpy(sa.sa_data, DEVICE_NAME);
					
			if((sendlen = sendto(sockfd_send, (void*)snd_pak, sizeof(struct arp_packet), 0, &sa, sizeof(struct sockaddr)))<0){
				perror("sendto");
				exit(1);
			}
			else{
				printf("Send ARP Reply : %s is %s\n", get_sender_protocol_addr(&(snd_pak->arp)), get_sender_hardware_addr(&(snd_pak->arp)));
				printf("Send successfull.\n");
				fflush(stdout);
			}
		}
		
	}
	
	return 0;
}