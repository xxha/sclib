#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include <stdio.h>

int main()
{
	int fa;
	int s=12;
	int len;
	char text[20]="hello,fileopen2!!!";

	fa=open("fileopen2.txt",O_WRONLY|O_CREAT,0640);

        //len = sprintf(text, "%d", s);
	write(fa,text,sizeof(text));
	close(fa);
}
