/*
 * This pragram used to show how char pointer and array connected.
*/
#include <stdio.h>
#include <string.h>

int test1()
{
	char array[128];
	char *test;

	test = array;

	memset(array, 0, 128);

	test[0] = 'a';
	test[1] = 'b';
	test[2] = 'c';

	printf("test =%s-----\n", test);

	return 0;
}

/* This program shows how to locate string element addrees by pointer and array. */
int test2()
{
	char *test = "hello world";

	printf("test + 2 = %c --- %c.\n", *(test + 2), test[2]);

	return 0;
}

int main()
{
	test1();
	test2();

	return 0;
}
