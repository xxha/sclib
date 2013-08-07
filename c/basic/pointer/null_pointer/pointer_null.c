#include <stdio.h>

typedef struct {
	unsigned char *p;
	int i;
	char y[8];
}TEST;

int test(unsigned char *p)
{
	printf("p = %c\n", *p);

}

int main()
{
//	TEST *test1;

	unsigned char *p;
	*p = '1';
	test(p);	

}


