#include <stdio.h>
#include <string.h>

#define WORDS "beast"
#define SIZE 40

int main(void)
{
	char *orig = WORDS;
	char copy[SIZE] = "Be the best that you can be.";
	char *ps;

	puts(orig);
	puts(copy);
	ps = strcpy(copy + 7, orig); //copy + 7, 表示从 copy + 7 的位置开始拷贝
	puts(copy);

	puts(ps);      //返回之ps 是第一个参数的值， 也就是copy + 7 位置
	return 0;	
}
