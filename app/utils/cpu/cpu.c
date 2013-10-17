/*
 * CPU occupancy rate operations
 */

#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;

	for(; ;) {
		for(i = 0; i < 9600000; i++)
			;
		usleep(24000);
	}
	return 0;
}


