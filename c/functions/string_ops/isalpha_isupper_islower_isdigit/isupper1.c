#include <ctype.h>
#include <stdio.h>

int main()
{
	char Test[]="a1B2c3D4";
	char *pos;
	pos=Test;

	while(*pos!=0) {
		if(isupper(*pos))
			printf("%c",*pos);
		pos++;
	}

	printf("\n");
	return 0;
}
