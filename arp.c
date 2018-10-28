#include "arp.h"

//You can fill the following functions or add other functions if needed. If not, you needn't write anything in them.  
void set_hard_type(struct ether_arp *packet, unsigned short int type){
	packet->arp_hrd = type;
}
void set_prot_type(struct ether_arp *packet, unsigned short int type){
	packet->arp_pro = type;
}
void set_hard_size(struct ether_arp *packet, unsigned char size){
	packet->arp_hln = size;
}
void set_prot_size(struct ether_arp *packet, unsigned char size){
	packet->arp_pln = size;
}
void set_op_code(struct ether_arp *packet, short int code){
	packet->arp_op = code;
}

void set_sender_hardware_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_sha, address, ETH_ALEN);
}
void set_sender_protocol_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_spa, address, 4);
}
void set_target_hardware_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_tha, address, ETH_ALEN);
}
void set_target_protocol_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_tpa, address, 4);
}

char* get_target_protocol_addr(struct ether_arp *packet){
	struct in_addr target_addr;
	memcpy(&target_addr, packet->arp_tpa, 4);
	return inet_ntoa(target_addr);
}
char* get_sender_protocol_addr(struct ether_arp *packet){
	struct in_addr sender_addr;
	memcpy(&sender_addr, packet->arp_spa, 4);
	return inet_ntoa(sender_addr);
}
// char* get_sender_hardware_addr(struct ether_arp *packet)
// {
// 	// if you use malloc, remember to free it.
// }
// char* get_target_hardware_addr(struct ether_arp *packet)
// {
// 	// if you use malloc, remember to free it.
// }
char* get_locol_IP(){
	struct ifreq ifr;
    return inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);
}
char* get_local_MAC(){
	struct ifreq ifr;
	return ifr.ifr_hwaddr.sa_data;
}

void printHelp(){
	printf("Format :\n");
	printf("1) ./arp -l -a\n");
	printf("2) ./arp -l <filter_ip_address>\n");
	printf("3) ./arp -q <query_ip_address>\n");
	printf("4) ./arp <fake_mac_address> <target_ip_address>\n");
}