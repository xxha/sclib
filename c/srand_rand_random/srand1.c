/* output random data in 0-100 */

#include <stdlib.h> /* for srand() */
#include <stdio.h>
#define MAX 10
 
int main( void)
{
	int number[MAX] = {0};
	int i;
	unsigned int seed;

	scanf("%d",&seed);/*�ֶ���������*/
	srand(seed);

	for(i = 0; i < MAX; i++)
	{
		number[i] = rand() % 100; /*����100���ڵ��������*/
		printf("%d ", number[i]);
	}
	printf("\n");
	printf("=======================================================\n");

	for(i = 0; i < MAX; i++)
	{
		number[i] = random() % 100; /*����100���ڵ��������*/
		printf("%d ", number[i]);
	}
	printf("\n");
	printf("=======================================================\n");
	return 0;
}
