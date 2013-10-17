/*
 * use time seed to generate random data,
 * the outcome data should be different.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h> /* for time() */

#define MAX 10
 
int main( void)
{
	int number[MAX] = {0};
	int i;

	srand((unsigned) time(NULL)); /* set seed */

	for(i = 0; i < MAX; i++) {
		number[i] = rand() % 100; /* generate random int in 0-100*/
		printf("%d ", number[i]);
	}

	printf("\n");
	return 0;
}
