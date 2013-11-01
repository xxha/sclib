#include <stdio.h>
#include <string.h>

int main()
{
	int i;
	char mac[6]={0xFF, 0xFE, 0xFD, 0xFC, 0x9F, 0xFF};
	unsigned char mac1[6]={0xFF, 0xFE, 0xFD, 0xFC, 0x9F, 0xFF};

	memset(mac, 0, 6);
	mac[0] = 0xFF;
	mac[1] = 0xF9;
	mac[2] = 0x9F;
	mac[3] = 0x7F;
	mac[4] = 0x3F;
	mac[5] = 0xF1;

	printf("mac addr = ");
	for(i=0; i<6; i++)
		printf("%x:", mac[i]);
	printf("\n");

	printf("mac1 addr = ");
	for(i=0; i<6; i++)
		printf("%02x:", mac1[i]);
	printf("\n");

	return 0;
}
