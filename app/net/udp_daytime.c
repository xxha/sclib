/* Send a udp DGRAM socket to 192.168.8.164, to request datetime. */

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <linux/net.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 	150   /* arbitory size */

int main()
{
	struct 	sockaddr_in serv;
	char 	buff[BUFFSIZE];
	int 	sockfd, n;

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		printf("socket error\n");

	bzero((char*) &serv, sizeof(serv) );
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("192.168.8.1");
	serv.sin_port = htons(13);

	if(sendto(sockfd, buff, BUFFSIZE, 0,
			(struct sockaddr *)&serv, sizeof(serv)) != BUFFSIZE)
		printf("sendto error\n");

	if((n = recvfrom(sockfd, buff, BUFFSIZE, 0,
				(struct sockaddr *) NULL, (int *) NULL)) != BUFFSIZE)
		printf("recvfrom error\n");

	buff[n - 2] = '\0';
	printf("-----:%s\n", buff);

	return 0;
	
}
