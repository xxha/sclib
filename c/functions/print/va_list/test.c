#include <stdio.h>

#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

int main()
{
	int i = 0;
	char *var = "hello";

	printf("int size:----------------------------------");
	for (i=0; i<10; i++) {
		printf("INTSIZEOF(%d) = %d\n", i, _INTSIZEOF(i));
	}

	
	printf("char * size:----------------------------------");
		printf("INTSIZEOF(var) = %d\n", _INTSIZEOF(var));
	return 0;
}
