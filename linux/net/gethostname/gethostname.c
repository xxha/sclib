/*
 *	gethostname() --- 返回本地主机的标准主机名
 *
 *	int PASCAL FAR gethostname(char FAR *name, int namelen);
 *	name：   一个指向将要存放主机名的缓冲区指针。
 *	namelen：缓冲区的长度。
 */

#include <stdio.h>
#include <unistd.h>

int main()
{

	char name[65];
	gethostname(name, sizeof(name));
	printf("hostname = %s\n", name);
}
