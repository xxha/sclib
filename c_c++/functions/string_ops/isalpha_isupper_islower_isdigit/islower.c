#include <ctype.h>
#include <stdio.h>

int main()
{
	char str[]="123@#FDsP[e?";
	int i;

	for(i=0;str[i]!=0;i++)
		if(islower(str[i]))
			printf("%c is a lower-case character\n",str[i]);

	return 0;
}
