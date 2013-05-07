#include <stdio.h>

#define MIN(A, B) ((A) <= (B) ?(A):(B))

int main()
{
	int a = 2, b = 1;
	int c;

	c = MIN(a, b);
	printf("c = %d\n", c);


	return c;
}
