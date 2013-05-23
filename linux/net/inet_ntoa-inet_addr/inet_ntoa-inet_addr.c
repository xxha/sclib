#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef struct sockaddr_in SOCKADDR_IN;

main()
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.s_addr=inet_addr("127.0.0.1");  //inet_addr()

	//将sin_addr储存的IP(127.0.0.1)转换成字符串形式。
	printf("IP = %s\n",inet_ntoa(addrSrv.sin_addr));

}

