#include <stdio.h>

int main(void)
{
	int i = 0x12345678;
	int y = 0;

	printf("sizeof int = %d \n", sizeof(int));
	printf("sizeof short int = %d \n", sizeof(short int));
	printf("sizeof long int = %d \n", sizeof(long int));
	printf("sizeof long long = %d \n", sizeof(long long));

	printf("x = 0x%x.\n", i);

	y = htons(i);
	printf("y = htons(i) = 0x%x .\n", y);

	y = htonl(i);
	printf("y = htonl(i) = 0x%x.\n", y);

	y = ntohs(i);
	printf("y = ntohs(i) = 0x%x.\n", y);
	
	y = ntohl(i);
	printf("y = ntohl(i) = 0x%x.\n", y);

	return 0;
}
