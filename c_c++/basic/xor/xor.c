/*
 *  use xor to exchange a and b, no other variable.
 */

#include <stdio.h>

int main()
{
	int a = 0x11000011;
	int b = 0x10100101;

	printf("old a = %x, b = %x\n", a, b);

	a = a ^ b;
	b = b ^ a;
	a = a ^ b;

	printf("new a = %x, b = %x\n", a, b);
	return 0;
}

