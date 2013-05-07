/*
 *    base sort    
 *    example sort data��12、 104、 13、 7、 9
 *
 *    1. by unit sort: 12、13、104、7、9
 *    2. by decade sort: 104、7、9、12、13
 *    3. by hundreds sort: 7、9、12、13、104
 *    4. by thousands sort:...  
 *    5.  ....
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* sort data in specified digit */
int pre_process_data(int array[], int length, int weight)
{
	int index;
	int value = 1;

	for(index = 0; index < weight; index++)
		value *= 10;

	for(index = 0; index < length; index++)
		array[index] = array[index] % value /(value /10);

	for(index = 0; index < length; index++)
		if(0 != array[index])
			return 1;

	return 0;
}

/* sort 0~10 */
void sort_for_basic_number(int array[], int length, int swap[])
{
	int index;
	int basic;
	int total = 0;

	for(basic = -9; basic < 10; basic++){
		for(index = 0; index < length; index++){
			if(-10 != array[index] && basic == array[index] ){
				swap[total ++] = array[index];
				array[index] = -10;
			}
		}
	}

	memmove(array, swap, sizeof(int) * length);
}

void sort_data_by_basic_number(int array[], int data[], int swap[], int length, int weight)
{
	int index;
	int outer;
	int inner;
	int value = 1;

	for(index = 0; index < weight; index++)
		value *= 10;

	for(outer = 0; outer < length; outer++){
		for(inner = 0; inner < length; inner++){
			if(-10 != array[inner] && data[outer]==(array[inner] % value /(value/10))){
				swap[outer] = array[inner];
				array[inner] = -10;
				break;
			}
		}
	}

	memmove(array, swap, sizeof(int) * length);
	return;
}

void radix_sort(int array[], int length)
{
	int* pData;
	int weight = 1;
	int count;
	int* swap;
	if(NULL == array || 0 == length)
		return;

	pData = (int*)malloc(sizeof(int) * length);
	assert(NULL != pData);
	memmove(pData, array, length * sizeof(int));

	swap = (int*)malloc(sizeof(int) * length);
	assert(NULL != swap);

	while(1){
		count = pre_process_data(pData, length, weight);
		if(!count)
			break;

		sort_for_basic_number(pData, length, swap);
		sort_data_by_basic_number(array, pData, swap, length, weight);
		memmove(pData, array, length * sizeof(int));
		weight ++;
	}

	free(pData);
	free(swap);
	return;
}

int main(void)
{
	int array[] = {3, 2, 104, 506, 707, 1102, 4403, 501, 403, 789, 111, 4, 5, 99, 13};
	int i;

	radix_sort(array, sizeof(array)/sizeof(int));
	printf("array[] = \n");
	for (i = 0; i < sizeof(array)/sizeof(int); i++)
		printf("%d, ", array[i]);
	printf("\n");
}








