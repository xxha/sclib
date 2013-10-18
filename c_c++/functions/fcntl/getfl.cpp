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
	int fd, flags;

	fd = open(argv[1],O_RDWR);
	if (argc!=2)
	{
		perror("--");
		cout<<"please input argument, which is filename."<<endl;
		return -1;
	}

	printf("fd = 0x%d\n", fd);

	/* read the flags */
	if((flags = fcntl(fd, F_GETFL, 0))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}
	printf("before change :flags = 0x%x\n", flags);

	switch(flags & O_ACCMODE)
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

	if (flags & O_APPEND)
		cout<<",append"<<endl;

	if (flags & O_NONBLOCK)
		cout<<",noblocking"<<endl;

	/* set nonblock flag */
	flags |= O_NONBLOCK;
	if((fcntl(fd, F_SETFL, flags))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}

	/* set append flag */
	flags |= O_APPEND;
	if((fcntl(fd, F_SETFL, flags | O_APPEND)) < 0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}

	
	if((flags=fcntl(fd, F_GETFL, 0))<0)
	{
		strerror(errno);
		cout<<"fcntl file error."<<endl;
	}

	printf("after change :flags = 0x%x\n", flags);
	if (flags & O_APPEND)
		cout<<"after change: append"<<endl;

	if (flags & O_NONBLOCK)
		cout<<"after change: noblocking"<<endl;

	exit(0);
}
