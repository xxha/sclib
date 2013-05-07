#include <stdio.h>

int main(void)
{
	FILE *fp;
	int fd;

	fp = fopen("/etc/passwd", "r");
	fd = fileno(fp);
	printf("fd = %d\n", fd);

	fclose(fp);
	return 0;
}
