#include <stdio.h>

int main(void)
{
	char name1[11], name2[11];
	int count;

	printf("Please input 2 name.\n");

	count = scanf("%5s %10s", name1, name2); 
	//count  ＝ 读入单词的个数

	printf("I read the %d names %s and %s.\n", count, name1, name2);

	return 0;
}
