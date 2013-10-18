/*gettype.c
 *
 * Get SO_TYPE Option:
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
	if(errno!=0)
	{
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
	int s = -1;			/* Socket */
	int so_type = -1;		/* Socket type */
	socklen_t optlen;		/* Option length */

	/*
	 * Create a TCT/IP socket to use:
	 */
	s = socket(PF_INET,SOCK_STREAM,0);
	if(s==-1)
		bail("socket(2)");

	/*
	 * Get socket option SO_TYPE:
	 */
	optlen = sizeof so_type;
	z = getsockopt(s,SOL_SOCKET,SO_TYPE,&so_type,&optlen);
	if(z)
		bail("getsockopt(s,SOL_SOCKET,""SO_TYPE)");
	assert(optlen == sizeof so_type);

	/*
	 * Report the result:
	 */
	printf("Socket s: %d\n",s);
	printf(" SO_TYPE : %d\n",so_type);
	printf(" SO_STREAM = %d\n",SOCK_STREAM);

	close(s);
	return 0;
}
