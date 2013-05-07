/*
 * This is a program to show opendir(), readdir(), closedir().
 * The function of the program is to list the folders under the request dir
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef DT_DIR
#error "DT_DIR not defined, maybe d_type not a mumber of struct dirent!"
#endif

int main(int argc, char*argv[])
{
	static char dot[] =".", dotdot[] ="..";
	const char *name;
	DIR *dirp;
	struct dirent *dp;

	if (argc == 2)
		name = argv[1];
	else
		name = dot;
	printf(" the request dir name is %s\n", name);

        //open the request dir.
	//return DIR pointer if open succeed.
	//return NULL if opendir failed.
	dirp = opendir(name);
	if (dirp == NULL) {
		fprintf(stderr, "%s: opendir(): %s: %s\n",
			argv[0], name, strerror(errno));
		exit(errno);
	} else {
		printf("opendir %s succeed!\n", name);	
	}


	//readdir(dirent)
	//return dirent pointer if readdir succeed.
	//return NULL if readdir failed.
	while ((dp = readdir(dirp)) != NULL) {
		//判断文件类型是DT_DIR, 也就是目录
		if (dp->d_type == DT_DIR)
			//如果文件名不是"."　或者"..",就打印出来
			if ( strcmp(dp->d_name, dot)
				&& strcmp(dp->d_name, dotdot) )
				printf("%s/\n", dp->d_name);
	}

	//closedir(dirent)
	closedir(dirp);
	return (0);
}
