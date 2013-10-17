#include <stdio.h>

#define MAX 81

int main(void)
{
	char name[MAX];
	char *ptr;

	printf("Hi, what's your name?\n");
	fgets(name, MAX, stdin);

	puts("Ah! "); 
	fputs(name, stdout);
	puts(" is a good name!\n");

	return 0;
}
