/*
 *    heap sort
 *
 *    1. array[n] >= array[2*n]
 *    2. array[n] >= array[2 * n + 1]
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>   /*assert(int expression)*/
#include <stdlib.h>   /*malloc() and free()*/

void heap_sort(int array[], int length)
{
	if(NULL == array || 0 == length)
		return ;

	/* to make sure data starts at number 1 */
	_heap_sort(array-1, length);
}

void _heap_sort(int array[], int length)
{
	int index = 0;
	int median = 0;
	construct_big_heap(array, length);

	for(index = length; index > 1; index --)
	{
		median = array[1];
		array[1] = array[index];
		array[index] = median;

		reconstruct_heap(array, 1, index-1);
	}
}

/* build big heap*/
void set_sorted_value(int array[], int length)
{
	int index = length;
	int median = 0;
	if(length == 1) return;

	while(index > 1){
		if(array[index >> 1] >= array[index])
			break;

		median = array[index];
		array[index] = array[index >> 1];
		array[index >> 1] = median;
		index >>= 1;
	}
}

void construct_big_heap(int array[], int length)
{
	int index = 0 ;

	for(index = 1; index <= length; index ++)
	{
		set_sorted_value(array, index);
	}
}

void reconstruct_heap(int array[], int index, int length)
{
	int swap = 0;
	if(length < index << 1)
		return;

	if(length == index << 1){
		adjust_leaf_position(array, index);
		return;
	}

	if(-1 != (swap = adjust_normal_position(array, index))){
		reconstruct_heap(array, swap, length);
	}
}

int adjust_normal_position(int array[], int index)
{
	int left = index << 1 ;
	int right = left + 1;
	int median = 0;
	int swap = 0;

	if(array[index] >= array[left]){
		if(array[index] >= array[right]){
			return -1;
		}else{
			swap = right;
		}
	}else{
		if(array[index] >= array[right]){
			swap = left;
		}else{
			swap = array[left] > array[right] ? left : right;
		}
	}

	if(swap == left) {
		median = array[index];
		array[index] = array[left];
		array[left] = median;
	}else{
		median = array[index];
		array[index] = array[right];
		array[right] = median;
	}

	return swap;
}

void adjust_leaf_position(int array[], int index)
{
	int median = 0;
	if(array[index] > array[index << 1])
		return;

	median = array[index];
	array[index] = array[index << 1];
	array[index << 1] = median;
	return;
}

static void test1()  
{  
	int array[] = {1};  
	heap_sort(array, sizeof(array)/sizeof(int));  
}  
  
static void test2()  
{  
	int array[] = {2, 1};  
	heap_sort(array, sizeof(array)/sizeof(int));  
	assert(1 == array[0]);  
	assert(2 == array[1]);  
}  
  
static void test3()  
{  
	int i;
	int array[] = {4, 3, 2,1,5,6,7,8,5,6,7,9,100,200,400, 19, 30, 20, 15};  
	heap_sort(array, sizeof(array)/sizeof(int));  
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
	heap_sort(array, sizeof(array)/sizeof(int));  
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

 
