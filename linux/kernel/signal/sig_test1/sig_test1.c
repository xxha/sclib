#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void sigroutine(int dunno)
{ /* 信号处理例程，其中dunno将会得到信号的值 */
	switch (dunno) {
	case 1:
		printf("Get a signal -- SIGHUP\n");
		break;
	case 2:
		printf("Get a signal -- SIGINT\n");
		break;
	case 3:
		printf("Get a signal -- SIGQUIT\n");
		break;
	}
	return;
}

int main() {
	printf("process id is %d \n",getpid());

	signal(SIGHUP, sigroutine); //* 下面设置三个信号的处理方法

	signal(SIGINT, sigroutine);

	signal(SIGQUIT, sigroutine);

	while(1);
} 
