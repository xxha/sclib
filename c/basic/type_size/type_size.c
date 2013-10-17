#include <stdio.h>

int main(void)
{
	unsigned int zero = 0;
	long long compzero = 0xFFFFFFFFFFFFFFFF;

	unsigned int zero1 = ~0;

	printf("size of short = %d\n", sizeof(short));
	printf("size of int = %d\n", sizeof(int));
	printf("size of short int = %d\n", sizeof(short int));
	printf("size of unsigned int = %d\n", sizeof(unsigned int));
	printf("size of unsigned long = %d\n", sizeof(unsigned long));
	printf("size of long = %d\n", sizeof(long));
	printf("size of long long = %d\n", sizeof(long long));

	printf("--------------------------------------------\n");
	printf("zero = 0x%x, compzero = 0x%llx\n", zero, compzero);
	printf("zero1 = 0x%x\n", zero1);

	return 0;
}

