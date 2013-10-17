#include <stdio.h>

#define MAX 81

int main(void)
{
	char name[MAX];
	printf("Hi, what's your name?\n");

	gets(name);
	printf("%s is a nice name ^_^!\n", name);

	return 0;
}
