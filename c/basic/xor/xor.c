/*
 *  use xor to exchange a and b, no other variable.
 */

#include <stdio.h>

int main()
{
	int a = 0x11000011;
	int b = 0x10100101;

	printf("old a = %d, b = %d\n", a, b);

	a = a ^ b;
	b = b ^ a;
	a = a ^ b;

	printf("new a = %d, b = %d\n", a, b);
	return 0;
}

