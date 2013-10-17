/*
 * This program shows how null pointer lead to program running broken.
*/

#include <stdio.h>

int main()
{

/* if *a = NULL, run this will outcome "Segmentation fault (core dumped)" */

//	int *a = NULL;
	int *a = "hello world";

	printf("a = %s\n", a);

	return 0;
}
