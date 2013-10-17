#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *a="-100.23";
	char *b="200e-2";
	float c;

	printf("atof(a)=%.2f\n", atof(a));
	printf("atof(b)=%.2f\n", atof(b));

	c=atof(a)+atof(b);
	printf("c=%.2f\n",c);

	return 0;
}
