/*
 * This program shows how to assign string or charactor to pointer.
*/

#include <stdio.h>

int test_char(char *p)
{
	printf("p = %c\n", *p);
	printf("p = %d\n", *p);
	return 0;
}

/* %c require int type parameter.*/
int test_int(int p)
{
	printf("p = %c\n", p);
	return 0;
}

int test_string(char *p)
{
	printf("p = %s\n", p);
	return 0;
}

int main()
{
	char p = '1';
	test_char(&p);

	/* int 65 means print %c output = A */
	int t = 65;
	test_int(t);

	char *q = "hello world";
	test_string(q);

	return 0;
}


