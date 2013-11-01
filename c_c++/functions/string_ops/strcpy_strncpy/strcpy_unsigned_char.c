#include <string.h>
#include <stdio.h>

int main()
{
	char *src = "255.255.255.0";
	char dest[256];

	memset(dest, 0, 256);

	strcpy(dest, src);
//	memcpy(dest, src, sizeof(src));

	printf("dest = %s\n", dest);

	return 0;
}
