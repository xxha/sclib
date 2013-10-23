#include <time.h> 
#include <memory.h> 
#include <stdio.h>

int main()
{

	struct timespec time;

	memset(&time, 0, sizeof(time));

	clock_gettime(CLOCK_REALTIME, &time);

	printf("%d----->%ld\n", time.tv_sec, time.tv_nsec);

	return 0;

} 
