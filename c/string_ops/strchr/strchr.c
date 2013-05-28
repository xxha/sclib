#include <string.h>
#include <stdio.h>

int main(void)
{
	char string[17];
	char *ptr, c = 'r';

	strcpy(string, "This is a string");

	ptr = strchr(string, c);
	if (ptr) /* ptr - string, means the address offset from string to ptr. */
		printf("The character %c is at position: %d\n", c, ptr-string);
	else
		printf("The character was not found\n");
	return 0;
}

/* define of strchr() */
#if 0
char* strchr(char* s,char c)
{
	while(*s != '\0' && *s != c) {
		++s;
	}
	return *s == c ?s:NULL;
}
#endif
