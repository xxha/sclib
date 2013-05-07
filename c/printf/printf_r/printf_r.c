#include <stdio.h>

int main()
{
	int i;

	for(i = 0; i < 10; i++) {
		printf("%d---------------%d\r",i, i);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
