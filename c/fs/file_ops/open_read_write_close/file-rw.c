#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fdr,fdw;
	char temp[255];
	char filepath[255];
	char fileto[255];

	printf("please input the filepath:");

	scanf("%s",filepath);

	if((fdr = open(filepath,O_RDONLY)) == -1) {
		printf("file not found,open failed!");
		return -1;
	} else {
		int r_size=read(fdr,temp,255);
		close(fdr);
		sprintf(fileto,"file-rw.txt");
		if((fdw=open(fileto,O_WRONLY|O_CREAT,0640))==-1) {
			printf("open failed!");
			return -1;
		} else {
			int w_size=write(fdw,temp,r_size);
			close(fdw);
			printf("have successfully copied 255 chars from \"%s\" to \"%s\"\n",filepath,fileto);
		}
	}
	return 0;
}
