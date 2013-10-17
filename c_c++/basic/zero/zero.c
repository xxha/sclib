#include <stdio.h>

int main ()
{
	unsigned int zero = 0;
	long long compzero = 0xFFFFFFFFFFFFFFFF;

	unsigned int zero1 = ~0;

	printf("zero = 0x%x\n \rcompzero = 0x%llx\n", zero, compzero);
	printf("zero1 = 0x%x\n", zero1);

	return 0;
}

