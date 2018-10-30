#include "arp.h"

void set_ether_dst_addr(struct ether_header *packet, char *address){
	struct ether_addr dst_addr;
	ether_aton_r(address, &dst_addr);
	memcpy(packet->ether_dhost, &dst_addr, ETH_ALEN);
	printf("%s\n", address);
}
void set_ether_src_addr(struct ether_header *packet, char *address){
	struct ether_addr src_addr;
	ether_aton_r(address, &src_addr);
	memcpy(packet->ether_shost, &src_addr, ETH_ALEN);
	printf("%s\n", address);
}

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
	printf("%s\n", address);
}
void set_sender_protocol_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_spa, address, 4);
	printf("%s\n", address);
}
void set_target_hardware_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_tha, address, ETH_ALEN);
	printf("%s\n", address);
}
void set_target_protocol_addr(struct ether_arp *packet, char *address){
	memcpy(packet->arp_tpa, address, 4);
	printf("%s\n", address);
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
char* get_target_hardware_addr(struct ether_arp *packet)
{
	struct ether_addr target_mac;
	char *MAC = malloc(sizeof(char)*18);
	memcpy(&target_mac, packet->arp_tha, 6);
	ether_ntoa_r(&target_mac ,MAC);
	return MAC;//reture an memory address
}
char* get_locol_IP(){
	int fd;
	struct ifreq ifr;
	char *iface = "enp0s3";

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);

	return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}
char* get_local_MAC(){
	int fd;
    struct ifreq ifr;
    char *iface = "enp0s3";
    unsigned char *MAC;
 
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
 
    ioctl(fd, SIOCGIFHWADDR, &ifr);
    close(fd);
 
    MAC = (unsigned char *)ifr.ifr_hwaddr.sa_data;
 
    char *str_MAC = malloc(sizeof(char)*18);
    sprintf(str_MAC, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
 
    return str_MAC;
}

void printHelp(){
	printf("Format :\n");
	printf("1) ./arp -l -a\n");
	printf("2) ./arp -l <filter_ip_address>\n");
	printf("3) ./arp -q <query_ip_address>\n");
	printf("4) ./arp <fake_mac_address> <target_ip_address>\n");
}