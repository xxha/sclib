
#include <stdio.h>
#include <string.h>
#include <assert.h>   

static int gQuickSort[] = {0};

void _quick_sort(int array[], int start, int end)  
{  
	int middle;  
	if(start >= end)  
		return;  
  
	middle = get_middle(array, start, end);  
	_quick_sort(array, start, middle -1);  
	_quick_sort(array, middle + 1, end);  
}  

void quick_sort(int array[], int length)  
{  
	int median = 0;  
	if(NULL == array || 0 == length)  
		return;  
  
	_quick_sort(array, 0, length -1);  
}  

int get_middle(int array[], int start, int end)  
{  
	int front = 0;  
	int tail = end - start;  
	int value = array[start];  
	int length = end - start + 1;  
	int loop = start + 1;  
  
	while(loop <= end){
		if(array[loop] < value){     
			gQuickSort[front] = array[loop];  
			front ++;  
		}else{                      
			gQuickSort[tail] = array[loop];  
			tail --;  
		}  
  
		loop ++;  
	}  

	//gQuickSort[] is a global array.  
	gQuickSort[front] = value; //value is the middle element.

	memmove(&array[start], gQuickSort, sizeof(int) * (length));

	return start + front ;  
}  

static void test1()  
{  
	int array[] = {1};  
	quick_sort(array, sizeof(array)/sizeof(int));  
}  
  
static void test2()  
{  
	int array[] = {2, 1};  
	quick_sort(array, sizeof(array)/sizeof(int));  
	assert(1 == array[0]);  
	assert(2 == array[1]);  
}  
  
static void test3()  
{  
	int i;
	int array[] = {4, 3, 2,1,5,6,7,8,5,6,7,9,100,200,400, 19, 30, 20, 15};  
	quick_sort(array, sizeof(array)/sizeof(int));  
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
	quick_sort(array, sizeof(array)/sizeof(int));  
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

 
