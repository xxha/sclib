#include <stdio.h>
#include <ctype.h>

main()
{
	int c;

	c='a';
	printf("%c:%s\n",c,isupper(c)?"yes":"no");

	c='A';
	printf("%c:%s\n",c,isupper(c)?"yes":"no");

	c='7';
	printf("%c:%s\n",c,isupper(c)?"yes":"no");

	return 0;
}
