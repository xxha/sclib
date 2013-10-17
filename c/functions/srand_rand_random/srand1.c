/* 
 * output random data in 0-100 by seed.
 * different seed outcome different number;
 * the same seed outcome the same number.
 *
*/

#include <stdlib.h> /* for srand() */
#include <stdio.h>
#define MAX 10
 
int main( void)
{
	int number[MAX] = {0};
	int i;
	unsigned int seed;
	int ret;

	ret = scanf("%d",&seed);/*手动输入种子*/
	printf("scanf input %d items\n", ret);
	printf("seed = %d\n", seed);

	srand(seed);

	for(i = 0; i < MAX; i++)
	{
		number[i] = rand() % 100; /*产生100以内的随机整数*/
		printf("%d ", number[i]);
	}
	printf("\n");
	printf("=======================================================\n");

	for(i = 0; i < MAX; i++)
	{
		number[i] = random() % 100; /*产生100以内的随机整数*/
		printf("%d ", number[i]);
	}
	printf("\n");
	printf("=======================================================\n");
	return 0;
}
