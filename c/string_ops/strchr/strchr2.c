#include <stdio.h>
#include <string.h>
int main()
{
	char temp[32];

	memset(temp,0,sizeof(temp));

	strcpy(temp,"Golden Global View");
	char *s = temp;
	char *p,c='V';

	p=strchr(s,c);
	if(p)
		printf("%s\n",p);
	else
		printf("Not Found!\n");
	return 0;
}
