/*
 * print app location start address and end address.
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int main()
{
	pid_t p;
	char cmd[BUFFER_SIZE];
	int ret;

	printf("app start address is: 0x%p\n", &main);

	p = getpid();
	printf("pid of this app is %d\n", p);

	memset(cmd, 0, BUFFER_SIZE);
	sprintf(cmd, "pmap -d %d", p);
	printf("cmd contents is : %s\n", cmd);
	ret = system(cmd);
	if(ret != 0)
		printf("%s failed\n", cmd);	

	printf("----------------------------------------------------\n");

	memset(cmd, 0, BUFFER_SIZE);
	sprintf(cmd, "cat /proc/%d/maps", p);
	printf("cmd contents is : %s\n", cmd);
	ret = system(cmd);
	if(ret != 0)
		printf("%s failed\n", cmd);	

	printf("----------------------------------------------------\n");
	memset(cmd, 0, BUFFER_SIZE);
	sprintf(cmd, "cat /proc/%d/smaps", p);
	printf("cmd contents is : %s\n", cmd);
	ret = system(cmd);
	if(ret != 0)
		printf("%s failed\n", cmd);	

	printf("----------------------------------------------------\n");

	memset(cmd, 0, BUFFER_SIZE);
	sprintf(cmd, "cat /proc/%d/statm", p);
	printf("cmd contents is : %s\n", cmd);
	ret = system(cmd);
	if(ret != 0)
		printf("%s failed\n", cmd);	

	printf("----------------------------------------------------\n");
	return 0;	
}
