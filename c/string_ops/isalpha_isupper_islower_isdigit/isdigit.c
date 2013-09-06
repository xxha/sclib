#include <stdio.h>
#include <ctype.h>

int main()
{
	int c;

	c='a';
	printf("%c:%s\n",c,isdigit(c)?"yes":"no");

	c='9';
	printf("%c:%s\n",c,isdigit(c)?"yes":"no");

	c='*';
	printf("%c:%s\n",c,isdigit(c)?"yes":"no");

	return 0;
}
