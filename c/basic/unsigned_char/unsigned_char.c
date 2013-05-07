#include <stdio.h>
#include <string.h>

int main(int argc, char **argv[])
{
	unsigned char test[4] = {0};

	test[0] = 33;
	test[1] = 2;
	test[2] = 2;
	test[3] = 33;

	printf("test = 0x%c\n", test[0]);
	printf("test = 0x%x\n", test[1]);
	printf("test = 0x%x\n", test[2]);
	printf("test = 0x%x\n", test[3]);
}
