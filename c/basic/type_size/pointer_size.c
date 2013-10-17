#include <stdio.h>

int main()
{
	short **presscode;

	printf("size of (void *) = %d\n", sizeof(void *));

	printf("size of presscode[0] = %d\n", sizeof(presscode[0]));
	printf("size of presscode = %d\n", sizeof(presscode));
	printf("size of *presscode = %d\n", sizeof(*presscode));
	printf("size of **presscode = %d\n", sizeof(**presscode));
	printf("size of presscode[0][0] = %d\n", sizeof(presscode[0][0]));

	printf("we have a conclusion: pointer size = 4\n");

	return 0;
}
