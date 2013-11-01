#include <stdio.h>

int main()
{
	unsigned int addr[4];
	char *p;
	unsigned char *up;

	addr[0] = 0xFFFCED19;

	up = (unsigned char *) addr;
	printf("p[0] = %d\n", *up);
	printf("p[1] = %d\n", *(up + 1));
	printf("p[2] = %d\n", *(up + 2));
	printf("p[3] = %d\n", *(up + 3));

	p = (char *) addr;
	printf("p[0] = %d\n", *p);
	printf("p[1] = %d\n", *(p + 1));
	printf("p[2] = %d\n", *(p + 2));
	printf("p[3] = %d\n", *(p + 3));

	return 0;
}
