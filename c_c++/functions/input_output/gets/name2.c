#include <stdio.h>

#define MAX 81

int main(void)
{
	char name[MAX];
	char *ptr;

	printf("Hi, what's your name?\n");

	ptr = gets(name);

	printf("%s? Ah! %s is a nice name ^_^!\n", name, ptr);

	return 0;
}