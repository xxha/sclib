/*setsndrcv.c
 *
 * Set SO_SNDBUF & SO_RCVBUF Options:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>

/*
 * This function report the error and
 * exits back to the shell:
 */
static void bail(const char *on_what)
{
	if(errno!=0) {
		fputs(strerror(errno),stderr);
		fputs(": ",stderr);
	}

	fputs(on_what,stderr);
	fputc('\n',stderr);
	exit(1);
}

int main(int argc,char **argv)
{
	int z;
	int s=-1;			/* Socket */
	int sndbuf=0;		/* Send buffer size */
	int rcvbuf=0;		/* Receive buffer size */
	socklen_t optlen;		/* Option length */

	/*
	 * Create a TCP/IP socket to use:
	 */
	s = socket(PF_INET,SOCK_STREAM,0);
	if(s==-1)
	bail("socket(2)");

	/*
	 * set the SO_SNDBUF size :
	 */
	sndbuf = 5000;	/* Send buffer size */
	z = setsockopt(s,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof sndbuf);
	if(z)
		bail("setsockopt(s,SOL_SOCKET,""SO_SNDBUF)");

	/*
	 * Set the SO_RCVBUF size:
	 */
	rcvbuf = 8192;	/* Receive buffer size */
	z = setsockopt(s,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof rcvbuf);
	if(z)
		bail("setsockopt(s,SOL_SOCKET,""SO_RCVBUF)");

	/*
	 * As a check on the above ....
	 * Get socket option SO_SNDBUF:
	 */
	optlen = sizeof sndbuf;
	z = getsockopt(s,SOL_SOCKET,SO_SNDBUF,&sndbuf,&optlen);
	if(z)
		bail("getsockopt(s,SOL_SOCKET,""SO_SNDBUF)");

	assert(optlen == sizeof sndbuf);

	/*
	 * Get socket option SO_RCVBUF:
	 */
	optlen = sizeof rcvbuf;
	z = getsockopt(s,SOL_SOCKET,SO_RCVBUF,&rcvbuf,&optlen);
	if(z)
	bail("getsockopt(s,SOL_SOCKET"
		"SO_RCVBUF)");
	assert(optlen == sizeof rcvbuf);

	/*
	 * Report the buffer sizes:
	 */
	printf("Socket s: %d\n",s);
	printf(" Send buf: %d bytes\n",sndbuf);
	printf(" Recv buf: %d bytes\n",rcvbuf);

	close(s);
	return 0;
}
