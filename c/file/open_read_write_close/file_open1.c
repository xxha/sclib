#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	char temp[] = "hello,abc!";
	int fd;
	char pathname[255];

	if((fd=open("fileopen.txt",O_WRONLY|O_CREAT,0640))==-1) {
		printf("creat file wrong!");
	}

	int len = strlen(temp) + 1;

	/*д���ļ���*/
	write(fd,temp,len);

	close(fd);
}
