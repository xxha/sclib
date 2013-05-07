/*
 *   merge sort steps:   
 *   1. seperate left array[] and right array[]
 *   2. iteration 1 
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>   /* assert(int expression) */
#include <stdlib.h>   /* malloc(),free() */

void merge_sort(int array[], int length)
{
	if(NULL == array || 0 == length)
		return ;
	_merge_sort(array, 0, length-1);
}

void _merge_sort(int array[], int start, int end)
{
	if(start >= end)
		return;

	int middle = start + ((end - start) >> 1);
	_merge_sort(array, start, middle);
	_merge_sort(array, middle + 1, end);
	_merge_data_in_array(array, start, middle, end);
}

void _merge_data_in_array(int array[], int start, int middle, int end)
{
	int length = end - start + 1;
	int* pData = NULL;
	int left = start;
	int right = middle + 1;
	int all = 0;

	/* allocate new memory to the space */
	pData = (int*) malloc(sizeof(int) * length);
	assert(NULL != pData);
	memset(pData, 0, length);

	/* begin to move data */
	while(right <= end){
		while(array[left] <= array[right] && left <= middle){
			pData[all] = array[left];          //copy array[left] to pData[all]
			left ++;
			all ++;  //use all to locate pData[all]
		}

		if(left > middle)  {
			break;
		}

		while(array[left] > array[right] && right <= end){
			pData[all] = array[right];        //copy array[right] to pData[all]
			right ++;
			all ++;
		}
	}

	/* move the left data */
	if(left <= middle)
		memmove(&pData[all], &array[left], sizeof(int) * (middle -left +1));

	if(right <= end)
		memmove(&pData[all], &array[right], sizeof(int) * (end - right + 1));
	
	memmove(&array[start], pData, sizeof(int) * length);
	free(pData);
}

static void test1()  
{  
	int array[] = {1};  
	merge_sort(array, sizeof(array)/sizeof(int));  
}  
  
static void test2()  
{  
	int array[] = {2, 1};  
	merge_sort(array, sizeof(array)/sizeof(int));  
	assert(1 == array[0]);  
	assert(2 == array[1]);  
}  
  
static void test3()  
{  
	int i;
	int array[] = {4, 3, 2,1,5,6,7,8,5,6,7,9,100,200,400, 19, 30, 20, 15};  
	merge_sort(array, sizeof(array)/sizeof(int));  
	assert(1 == array[0]);  
	assert(2 == array[1]);  
	assert(3 == array[2]);  
	assert(4 == array[3]);  
	printf("sorted test3 array: ");
	for(i = 0; i < sizeof(array)/sizeof(int); i++ )
		printf("%d ", array[i]);
	printf("\n");
}  
  
static void test4()  
{  
	int array[] = {3, 2, 1};  
	merge_sort(array, sizeof(array)/sizeof(int));  
	assert(1 == array[0]);  
	assert(2 == array[1]);  
	assert(3 == array[2]);  
} 

int main(void)
{
	test1();
	test2();
	test3();
	test4();

	printf("all tests complete\n");
	return 0;
}

 
