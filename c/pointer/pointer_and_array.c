#include <stdio.h>
#include <string.h>

int main()
{

	char *string="i love china";

	printf("%s \n",string);

	printf("%c  --  %c\n",*(string+2),string[2]);

	return 0;
}
