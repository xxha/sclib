#include <stdio.h>

int main()
{
	short test1;
	short **presscode;

	printf("size of short test1 = %d\n", sizeof(test1));
	printf("size of presscode[0] = %d\n", sizeof(presscode[0]));
	printf("size of short ** = %d\n", sizeof(presscode));
	printf("size of short * = %d\n", sizeof(*presscode));
	printf("size of short = %d\n", sizeof(**presscode));
	printf("size of presscode[0][0] = %d\n", sizeof(presscode[0][0]));

	printf("we have a conclusion: pointer size = 4, short size = 2\n");
	return 0;
}
