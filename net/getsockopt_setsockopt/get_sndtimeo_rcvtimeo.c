#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int sockfd;
	int timeout;
	socklen_t length;
	int ret;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("create sockfd failed.\n");
		return -1;
	}

	length = sizeof timeout;
	ret = getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, &length);
	if (ret) {
		printf("getsockopt SO_RCVTIMEO error %d, errno = %d\n", ret, errno);
		return ret;
	}

	printf("receive time out = %d\n", timeout);

	ret = getsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, &length);
	if (ret) {
		printf("getsockopt SO_SNDTIMEO error %d\n", ret);
		return ret;
	}

	printf("send time out = %d\n", timeout);

	close(sockfd);
	return 0;
}


