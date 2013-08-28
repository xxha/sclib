#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <linux/if.h> 
#include <arpa/inet.h> 
#include <linux/sockios.h> 
#include <sys/ioctl.h> 

int main(int argc, char **argv) 
{ 
	char buf[2048], ip_addr[16], mac_addr[16], netmask_addr[50];
	unsigned char *mac; 
	int sockFd, MTU; 
	struct ifconf ifconf; 
	struct ifreq *ifreq; 
	struct sockaddr_in *in_addr, *netmask; 
	int i;

	memset(ip_addr, 0, sizeof(ip_addr)); 

	sockFd = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sockFd < 0) 
	{ 
		perror("socket"); 
		exit(0); 
	} 

	ifconf.ifc_buf = buf; 
	ifconf.ifc_len = sizeof(buf); 

	if (ioctl(sockFd, SIOCGIFCONF, &ifconf) == 0) 
	{ 
		ifreq = ifconf.ifc_req; 
		/* get interfaces of system */
		for (i = 0; i < (int)(ifconf.ifc_len/sizeof(struct ifreq)); i++) { 
			if (ioctl(sockFd, SIOCGIFFLAGS, ifreq) == 0) { 
				printf("%s\n",ifreq->ifr_name); 
				in_addr = (struct sockaddr_in *)&ifreq->ifr_addr; 
				inet_ntop(AF_INET, &in_addr->sin_addr, ip_addr, 16); 
				printf("Addr: %s\n",ip_addr); 
			} 

			if (ioctl(sockFd, SIOCGIFMTU, ifreq) == 0) { 
				printf("MTU: %d\n", ifreq->ifr_mtu); 
			} 

			if (ioctl(sockFd, SIOCGIFNETMASK, ifreq) == 0) { 
				netmask = (struct sockaddr_in *)&(ifreq->ifr_netmask); 
				strcpy(netmask_addr, inet_ntoa(netmask->sin_addr)); 
				printf("Netmask: %s\n", netmask_addr); 
			} 
			if (ioctl(sockFd, SIOCGIFHWADDR, ifreq) == 0) { 
				mac =(unsigned char *) ifreq->ifr_hwaddr.sa_data; 
				printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", 
					mac[0],mac[1], 	mac[2], 
					mac[3], mac[4], mac[5]); 
			} 

			ifreq++; 
			printf("\n"); 
		} 
	} 
	return 0; 
}  
