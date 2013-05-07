#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>

int main(void)
{
	fd_set fdset;
	int i = 0;

	struct timeval time;
	time.tv_sec = 1;
	time.tv_usec = 0;

	FD_ZERO (&fdset);/*清空集合中所有的元素*/

	printf("stdout fileno = %d\n", STDOUT_FILENO);
	printf("stdin fileno = %d\n", STDIN_FILENO);


//	FD_SET(STDOUT_FILENO,&fdset);/*设置stdout，使集合中包含stdout*/
//	if(FD_ISSET(STDOUT_FILENO,&fdset)!=0)/*测试stdout是否包含在集合中*/
//		printf("stdout has been set\n");
//	else
//		printf("stdout has not been set\n");

	while( i < 5) {
		select(STDOUT_FILENO + 1, NULL, &fdset, NULL, &time);

		if(FD_ISSET(STDOUT_FILENO,&fdset)!=0)/*测试stdout是否包含在集合中*/
			printf("stdout has been set\n");
		else
			printf("stdout has not been set\n");
		i++;
	}

	FD_CLR(STDOUT_FILENO,&fdset);/*从位向量中清除stdout*/
	if(FD_ISSET(STDOUT_FILENO,&fdset)!=0)
		printf("stdout has been set\n");/*再次测试*/
	else
		printf("stdout has not been set\n");

	return 0;
}
