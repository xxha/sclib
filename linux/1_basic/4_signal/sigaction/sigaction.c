#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void show_handler(struct sigaction *act)
{
	if (act->sa_handler == SIG_DFL)
	{
		printf("Default action\n");
	} else if (act->sa_handler == SIG_IGN)
	{
		printf("Ignore the signal\n");
	} else
	{
		printf("0x%p\n",act->sa_handler);
	}
}

void main(void)
{
	int i, ret;
	struct sigaction act,oldact;

	act.sa_handler = (__sighandler_t)show_handler;
	act.sa_flags = SA_ONESHOT|SA_NOMASK;
	ret = sigaction(SIGINT, &act, &oldact);
	if(ret) {
		printf("sigaction failed 1\n");
	}

	for(i=5;i<15;i++) {
		printf("sa_handler of signal %2d = \n", i);
		ret = sigaction(i, NULL, &oldact);
	        if(ret) {
        	        printf("sigaction failed 2\n");
        	}
		sleep(1);
	}
}
