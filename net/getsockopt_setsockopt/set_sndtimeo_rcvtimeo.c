#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main()
{
	int sockfd;

	struct timeval rcv_time, snd_time;
	socklen_t length;
	int ret;

	struct timeval timeout = {3, 100000}; //precision = 4000us = 4ms =0.004s 

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("create sockfd failed.\n");
		return -1;
	}
	printf("sockfd = %d\n", sockfd);
	
	length = sizeof timeout;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, length);
	if (ret) {
		printf("setsockopt SO_RCVTIMEO error %d, errno = %d\n", ret, errno);
		return ret;
	}

	ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, length);
	if (ret) {
		printf("setsockopt SO_SNDTIMEO error %d, errno = %d\n", ret, errno);
		return ret;
	}

	length = sizeof rcv_time;
	ret = getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &rcv_time, &length);
	if (ret) {
		printf("getsockopt SO_RCVTIMEO error %d, errno = %d\n", ret, errno);
		return ret;
	}

	printf("receive time out = %d.%d\n", (int)rcv_time.tv_sec, (int)rcv_time.tv_usec);

	length = sizeof snd_time;
	ret = getsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &snd_time, &length);
	if (ret) {
		printf("getsockopt SO_SNDTIMEO error %d\n", ret);
		return ret;
	}

	printf("send time out = %d.%d\n", (int)snd_time.tv_sec, (int)snd_time.tv_usec);

	close(sockfd);
	return 0;
}


