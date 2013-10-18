#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <stdio.h> 	//perror()
#include <stdlib.h>     //atoi()
#include <string.h> 	//strerror()


using namespace std;

int main(int argc,char* argv[])
{
	int fd, var;

	fd=open(argv[1],O_RDWR);
	if (argc!=2)
	{
		perror("--");
		cout<<"please input argument, which is filename."<<endl;
		return -1;
	}

	printf("fd = 0x%d\n", fd);
	if((var=fcntl(fd, F_GETFL, 0))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}
	printf("before change :flags = 0x%x\n", var);

/* open/fcntl - O_SYNC is only implemented on blocks devices and on files
   located on a few file systems.  
#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 // not fcntl
#define O_EXCL             0200 // not fcntl
#define O_NOCTTY           0400 // not fcntl
#define O_TRUNC           01000 // not fcntl
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC         04010000
#define O_FSYNC          O_SYNC
#define O_ASYNC          020000
*/

	switch(var & O_ACCMODE)
	{
		case O_RDONLY : cout<<"Read only.."<<endl;
			break;
		case O_WRONLY : cout<<"Write only.."<<endl;
			break;
		case O_RDWR	 : cout<<"Read wirte.."<<endl;
			break;
		default	:
			break;
	}

	if (var & O_APPEND)
		cout<<",append"<<endl;

	if (var & O_NONBLOCK)
		cout<<",noblocking"<<endl;

	if((var=fcntl(fd, F_SETFL, O_NONBLOCK))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}
/*
	if((var=fcntl(fd, F_SETFL, O_APPEND))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}
*/
	printf("after change :flags = 0x%x\n", var);
	if (var & O_APPEND)
		cout<<"after change: append"<<endl;

	if (var & O_NONBLOCK)
		cout<<"after change: noblocking"<<endl;

	exit(0);
}
