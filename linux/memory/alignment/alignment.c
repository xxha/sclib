#include <stdio.h>

struct A {
	int a;
	char b;
	short c;
};

struct B {
	char b;
	int a;
	short c;
};

int main(void)
{

	printf("sizeof short = %d.\n", sizeof(short));
	printf("sizeof char = %d.\n", sizeof(char));
	printf("sizeof int = %d.\n", sizeof(int));
	printf("sizeof long = %d.\n", sizeof(long));
	printf("sizeof long long = %d.\n", sizeof(long long));
	printf("sizeof double = %d.\n", sizeof(double));
	printf("sizeof void * = %d.\n", sizeof(void *));


	printf("sizeof struct A = %d.\n", sizeof(struct A));
	printf("sizeof struct B = %d.\n", sizeof(struct B));

}



