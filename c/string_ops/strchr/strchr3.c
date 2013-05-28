#include <stdio.h>
#include <string.h>

void main()
{
	char answer[100],*p;

	printf("Type something:\n");
	fgets(answer,sizeof answer,stdin);

	if((p = strchr(answer,'\n')) != NULL)
		*p = '\0';
	printf("You typed \"%s\"\n",answer);
}
