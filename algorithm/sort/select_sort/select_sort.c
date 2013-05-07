/*
 *   select sort: select the minimum data and change.
 *
 *   2£¬ 1£¬ 5£¬ 4£¬ 9
 *
 *   fist time£º   1£¬ 2£¬ 5£¬ 4£¬ 9
 *   second time£º 1£¬ 2£¬ 5£¬ 4£¬ 9
 *   third time£º  1£¬ 2£¬ 4£¬ 5£¬ 9
 *   forth time£º  1£¬ 2£¬ 4£¬ 5£¬ 9
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>  

void select_sort(int array[], int length)
{
	int inner, outer, index, value, median;

	if(NULL == array || 0 == length)
		return;

	for(outer = 0; outer < length - 1; outer ++)
	{
		value = array[outer];
		index = outer;

		for(inner = outer +1; inner < length; inner ++){
			if(array[inner] < value){
				value = array[inner]; 
				index = inner;
			}
		}
		
		if(index == outer)
			continue;
		
		median = array[index];
		array[index] = array[outer];
		array[outer] = median;
	}
}

void print(int array[], int length)
{
	int index;
	if(NULL == array || 0 == length)
		return;

	for(index = 0; index < length; index++)
		printf("%d ", array[index]);
}

void test()
{
	int data[] = {2, 1, 5, 4, 9, 20, 500, 7, 8, 15, 300, 700, 20, 9, 7, 8};
	int size = sizeof(data)/sizeof(int);
	select_sort(data, size);
	print(data, size);
}

int main(void)
{
        
	printf("sorted array is :\n");
	test();
	printf("\n");
	return 0;
} 
