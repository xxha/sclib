#include <string.h>
#include <stdio.h>

/*
 *   (1) bubble sort
 */

#if 0
void bubble_sort(int array[], int length)
{
	int inner = 0, outer = 0;
	int median = 0;

	if(NULL == array || 0 == length)
		return;

	for(outer = length-1; outer >= 1; outer --){
		for(inner = 0; inner < outer; inner ++){
			if(array[inner] > array[inner + 1]){
				median = array[inner];
				array[inner] = array[inner + 1];
				array[inner + 1] = median;
			}
		}
	}
}
#endif

void bubble_sort(int array[], int length)
{
	int inner = 0, outer = 0;
	int median = 0;
	int flag = 1;

	if(NULL == array || 0 == length)
		return;

	for(outer = length-1; outer >= 1 && flag; outer --){
		flag = 0;

		for(inner = 0; inner < outer; inner ++){
			if(array[inner] > array[inner + 1]){
				median = array[inner];
				array[inner] = array[inner + 1];
				array[inner + 1] = median;

				if(flag == 0)
					flag = 1;
			}
		}
	}
}

/*
 *   insert sort
 */
void insert_sort(int array[], int length)
{
	int inner = 0;
	int outer = 0;
	int median = 0;
	if(NULL == array || 0 == length)
		return;

	for(outer = 1; outer <length; outer ++){
		for(inner = outer; inner >= 1; inner --){
			if(array[inner] < array[inner -1]){
				median = array[inner];
				array[inner] = array[inner -1];
				array[inner -1] = median;
			}else{
				break;
			}
		}
	}
}

/*
 * hash sort
 */
void shell_sort(int array[], int length, int step)
{
	int inner = 0;
	int outer = 0;
	int median = 0;
	int index;

	if(NULL == array || 0 == length)
		return;

	for(; step >= 1; step -=2){
		for(index = 0; index < step; index ++){
			if((length -1) < (index + step))
				continue;
			else{
				outer = index + step;
				while( (outer + step) <= (length - 1))
					outer += step;
			}

			for(;  outer >= (index + step);  outer -= step){
				for(inner = index; inner <= outer - step; inner += step){
					if(array[inner] >= array[inner + step]){
						median = array[inner];
						array[inner] = array[inner + step];
						array[inner + step] = median;
					}
				}
			}
		}
	}
}

int main(void)
{
	int array1[] = {100, 90, 40, 80, 10000, 500, 70, 900, 101, 89, 50000, 700, 865, 7, 20};
	int array2[] = {100, 90, 40, 80, 10000, 500, 70, 900, 101, 89, 50000, 700, 865, 7, 20};
	int array3[] = {100, 90, 40, 80, 10000, 500, 70, 900, 101, 89, 50000, 700, 865, 7, 20};
	int i;
	int step = 3;

	bubble_sort(array1, sizeof(array1)/sizeof(int));
	printf("bubble sort array1 = ");
	for(i = 0; i < sizeof(array1)/sizeof(int); i++)
		printf("%d ", array1[i]);
	printf(".\n");

	insert_sort(array2, sizeof(array2)/sizeof(int));
	printf("insert sort array2 = ");
	for(i = 0; i < sizeof(array2)/sizeof(int); i++)
		printf("%d ", array2[i]);
	printf(".\n");
	
	shell_sort(array3, sizeof(array3)/sizeof(int), step);
	printf("bubble sort array3 = ");
	for(i = 0; i < sizeof(array3)/sizeof(int); i++)
		printf("%d ", array3[i]);
	printf(".\n");

	return 0;
}


