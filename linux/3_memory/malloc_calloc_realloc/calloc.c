/*
 *   void *calloc(unsigned n,unsigned size)；
 *   功 能: 在内存的动态存储区中分配n个长度为size的连续空间，函数返回一个指向分配起始地址的指针；如果分配不成功，返回NULL。
 *   跟malloc的区别：calloc在动态分配完内存后，自动初始化该内存空间为零，而malloc不初始化，里边数据是随机的垃圾数据。
 *   并且malloc申请的内存可以是不连续的，而calloc申请的内存空间必须是连续的。
 *   头文件：stdlib.h或malloc.h
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#define MAIN1
#define MAIN2

#ifdef MAIN1
int main(void)
{
	char *str = NULL;

	/* 分配内存空间 */
	str = (char*)calloc(10, sizeof(char));

	/* 将hello写入*/
	strcpy(str, "Hello");

	/*显示变量内容*/
	printf("String is %s\n", str);

	/* 释放空间 */
	free(str);
	return 0;
}
#endif
#ifdef MAIN2
int main(void)
{
	int i;

	int *pn=(int *)calloc(10,sizeof(int));

	for(i=0;i<10;i++)
		printf("%3d",pn[i]);

	printf("\n");

	free(pn);
	return 0;
}

#endif
