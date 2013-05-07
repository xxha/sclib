#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int wc_file(char* filename)
{
	FILE* fp;
	char buf[1024];
	int lines;
	lines = 0;

	if((fp = fopen(filename,"r")) == NULL)
	{
		printf("fopen failed.\n");
		return 3;
	}

	while(fgets(buf,sizeof(buf),fp))
	{
		lines++;
	}
	fclose(fp);
	return lines;
}

int wc_dir(char* pathname)
{
	DIR* dp;
	struct dirent* pdirent;
	char filename[256];
	char dirname[256];
	int lines;
	lines = 0;

	if((dp = opendir(pathname)) == NULL)
	{
		printf("opendir %s failed.\n",pathname);
		return 4;
	}

	if(chdir(pathname) < 0)
	{
		printf("chdir %s failed.\n",pathname);
		//return 0;
	}

	while((pdirent = readdir(dp)) != 0)
	{
		if(pdirent->d_type == DT_REG)
		{
			strcpy(filename,pdirent->d_name);
			printf("filename: %s\n",filename);
			if(filename[strlen(filename)-2] != '.' || filename[strlen(filename)-1] != 'c')
			{
				continue;
			}
			lines += wc_file(filename);
		}
		else if(pdirent->d_type == DT_DIR)
		{
			strcpy(dirname,pdirent->d_name);
			if(strcmp(dirname,".") == 0 || strcmp(dirname,"..") == 0)
			{
				continue;
			}
			printf("directory: %s\n",dirname);
			char tmppath[256];
			//strcpy(tmppath,pathname);
			//strncat(tmppath,"/",1);
			strcpy(tmppath,dirname);
			lines += wc_dir(tmppath);  //递归
			chdir(pathname);
		}
		else
		{
			continue;
		}

	}
	chdir("..");
	closedir(dp);
	return lines;
}

int main(int argc,char* argv[])
{
	char* ptr;
	struct stat statbuf;
	int codelines;
	codelines = 0;

	if(argc != 2)
	{
		printf("usage: %s <name(file/path)>.\n",argv[1]);
		return 1;
	}

	printf("Welcome to chicken house!\n");
	if(lstat(argv[1],&statbuf) < 0)
	{
		printf("lstat failed.\n");
		return 2;
	}

	if((S_IFMT & statbuf.st_mode) == S_IFREG)
	{
		printf("input file is a regular file.\n");
		codelines = wc_file(argv[1]);
	}
	else if((S_IFMT & statbuf.st_mode) == S_IFDIR)
	{
		printf("input is a directory.\n");
		codelines = wc_dir(argv[1]);
	}
	else
	{
		switch(S_IFMT & statbuf.st_mode)
		{
			case S_IFREG: ptr = "regular";break;
			case S_IFDIR: ptr = "d"; break;
			case S_IFCHR: ptr = "char special";break;
			case S_IFBLK: ptr = "block special";break;
			case S_IFIFO: ptr = "fifo";break;
			case S_IFLNK: ptr = "symbolic link";break;
			case S_IFSOCK: ptr = "socket";break;
			default: ptr = "unknown mode";break;
		}
		printf("other file types: %s.\n",ptr);
	}
	printf("total code lines is %d.\n",codelines);
	return 0;
}
