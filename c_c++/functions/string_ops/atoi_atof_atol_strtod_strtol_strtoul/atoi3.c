#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i=0;
	unsigned char type[5];
	unsigned char *p;

	if (argc != 3) {
		printf("invalid argument\n");
		return -1;
	}

	printf("argc = %d\n", argc);
	
	memset(type, 0, 5);
	strcpy(type, argv[1]);
	printf("argv[1] = %s\n", argv[1]);
	printf("type = %s\n", type);

	i = atoi(argv[2]);
	printf("argv[2] = %s\n", argv[2]);
	printf("i = %d\n", i);


//	p = atoi(argv[1]);
//	printf("p = %s\n", p);

	return 0;
}
