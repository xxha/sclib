#include <signal.h>
#include <stdio.h>
#include <unistd.h>  //sleep()
#include <pthread.h>


void *ipc_monitor(void *param)
{
	if (param == NULL) {
		printf("param = null.\n");
	}
	while(1) {
		printf("hello, I'm zombie process!\n");
		sleep(5);
	}

	return 0;
}

int main()
{
	int ret;
	int param;
	struct sigaction sa;
	pthread_t thread_ipc;
	pthread_attr_t attr_ipc;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_NOCLDWAIT;

	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD, &sa, NULL);

	ret = pthread_attr_init(&attr_ipc);
	if(ret != 0) {
		printf("pthread_attr_init error.\n");
		return ret;
	}

	ret = pthread_create(&thread_ipc, &attr_ipc, ipc_monitor, &param);
	if(ret != 0) {
		printf("pthread_create error.\n");
		return ret;
	}

	while(1) {
		printf("hello, I'm father process!\n");
		sleep(5);
	}

	return 0;
}
