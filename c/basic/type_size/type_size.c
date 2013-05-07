#include <stdio.h>

int main ()
{
	unsigned int zero = 0;
	long long compzero = 0xFFFFFFFFFFFFFFFF;

	unsigned int zero1 = ~0;

	printf("int size = %d\n", sizeof(int));
	printf("short int size = %d\n", sizeof(short int));
	printf("unsigned int size = %d\n", sizeof(unsigned int));
	printf("unsigned long size = %d\n", sizeof(unsigned long));
	printf("long size = %d\n", sizeof(long));
	printf("long long size = %d\n", sizeof(long long));
	printf("zero = 0x%x, compzero = 0x%llx\n", zero, compzero);
	printf("zero1 = 0x%x\n", zero1);

	return 0;
}

