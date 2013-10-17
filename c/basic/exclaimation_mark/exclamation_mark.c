/*
 * This program shows how "!" and "~" charactor works.
 *
 * ! --- the exclamation mark
 * ~ --- I don't how say in english.
*/
#include <stdio.h>
#include <string.h>

int main()
{
	int i = 0;
	int j = 1;
	int k = -1;

	printf(" i = 0x%x \n", i);
	printf(" j = 0x%x \n", j);
	printf(" k = 0x%x \n", k);

	printf("--------------------------------------\n");

	printf(" !i = 0x%x \n", !i);
	printf(" !j = 0x%x \n", !j);
	printf(" !k = 0x%x \n", !k);

	printf("--------------------------------------\n");

	printf(" ~i = 0x%x \n", ~i);
	printf(" ~j = 0x%x \n", ~j);
	printf(" ~k = 0x%x \n", ~k);

	return 0;
}
