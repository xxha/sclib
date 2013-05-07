/*
 *  void *memset(void *s,int ch,size_t n); 
 *  函数解释：将 s 中前 n 个字节用 ch 替换并返回 s 
 *  memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法
 *  头文件 #include <string.h>
 *  返回值： s
 */

/*
    void *memcpy(void *dest, const void *src, size_t n);
    作用： 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
    头文件: #include <string.h>
    函数返回: dest的值。
*/


#include <string.h>
#include <stdio.h>

int main()
{
	char buf[30];
	char str1[10];
	char str2[20];
	char space = ' ';
	int i;

	memset(buf,0,sizeof(buf));
	printf("length of buf = %d\n", strlen(buf));
	printf("size of buf = %d\n", sizeof(buf));

	printf("please input str1 and str2\n");
	scanf("%s %s",str1, str2);
	printf("length of str1 = %d\n", strlen(str1));
	printf("length of str2 = %d\n", strlen(str2));
	printf("size of str1 = %d\n", sizeof(str1));
	printf("size of str2 = %d\n", sizeof(str2));

	memcpy(buf, str1, strlen(str1));
	memcpy(buf + strlen(str1), &space, 1);
	memcpy(buf + strlen(str1) + 1, str2, strlen(str2));

	printf("length of buf = %d\n", strlen(buf));
	printf("size of buf = %d\n", sizeof(buf));

	printf("after memcpy, buf = %s\n", buf);

	return 0;
} 

