/*
 * This program shows how to use define to make minimum and maximum function.
*/

#include <stdio.h>

#define MIN(A, B) ((A) <= (B) ? (A) : (B))
#define MAX(A, B) ((A) >= (B) ? (A) : (B))

int main()
{
	int a = 2, b = 1;
	int c, d;

	printf("a = %d, b = %d\n", a, b);

	c = MIN(a, b);
	printf("min of a, b = %d\n", c);

	d = MAX(a, b);
	printf("max of a, b = %d\n", d);

	return c;
}
