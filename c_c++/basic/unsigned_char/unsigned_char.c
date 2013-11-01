#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	unsigned char test[4] = {0};
	unsigned int test1 = 0;

	test1 -= 1;
	printf("test1 = %d\n", test1);

	test[0] = 33;
	test[1] = 2;
	test[2] = 2;
	test[3] = 33;

	printf("test[0] = %c\n", test[0]);
	printf("test[1] = 0x%x\n", test[1]);
	printf("test[2] = 0x%x\n", test[2]);
	printf("test[3] = 0x%x\n", test[3]);

	return 0;
}
