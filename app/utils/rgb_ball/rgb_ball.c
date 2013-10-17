#include <stdio.h>
#include <string.h>

#define SIZE 16

enum Color {
	RED,
	BLUE,
	GREEN,
};

typedef struct {
	unsigned int color;
	unsigned int weight;
}Ball;


unsigned int ColorOfBall(Ball *b)
{
	return b->color;
}

unsigned int WeightOfBall(Ball b)
{
	return b.weight;
}

Ball arr[SIZE] = {
	{0,10}, 
	{1,20}, 
	{2,130}, 
	{0,5}, 
	{1,10}, 
	{2,90}, 
	{2,9}, 
	{1,30}, 
	{0,8}, 
	{0,1}, 
	{2,5}, 
	{1,3}, 
	{1,7},
	{0,4},
	{2,13},
	{2,11}
};

Ball *ball_weight_sort(Ball *array, int len)
{
	int i,j;
	Ball temp;
#if 1
	for (i = len - 1; i > 0; i --) {
		for(j = 0; j < i; j++) {
			if(WeightOfBall(array[j]) > WeightOfBall(array[j+1])) {
				temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;

			}
		}
	}
#endif
	return array;

}


Ball *ball_sort(Ball *arr, int len)
{
	int i;
	int r = 0, b = 0, g = 0;
	Ball red_ball[SIZE], blue_ball[SIZE], green_ball[SIZE];
	memset(red_ball, 0, sizeof(Ball) * SIZE);
	memset(blue_ball, 0, sizeof(Ball) * SIZE);
	memset(green_ball, 0, sizeof(Ball) * SIZE);

	printf("length = %d\n", len);
	for (i = 0; i < len; i++) {
		switch (ColorOfBall(arr)) {
			case RED:
				red_ball[r] = *arr;
				r++;
				break;
			case BLUE:
				blue_ball[b] = *arr;
				b++;
				break;
			case GREEN:
				green_ball[g] = *arr;
				g++;
				break;
			default:
				continue;
		}
		arr++;
	}
	
	if((r + b + g) == len) {
		printf("the length is all right\n");
	}

	ball_weight_sort(red_ball, r);
	ball_weight_sort(blue_ball, b);
	ball_weight_sort(green_ball, g);

	memset(arr, 0, len* sizeof(Ball));

	for (i = 0; i < len; i++){
		if (i < r)
			arr[i] = red_ball[i];
		else if ((i >= r) && (i - r) < b)
			arr[i] = blue_ball[i-r];
		else if ((i >= (r + b)) && ((i - r - b) < g))
			arr[i] = green_ball[i - r - b];
	}

	printf("---------------------------------------------------\n");
	printf("the red ball is:\n");
	for(i = 0; i < r; i++)
		printf("red_ball[%d] = {%d, %d}\n", i, red_ball[i].color, red_ball[i].weight);

	printf("---------------------------------------------------\n");
	printf("the blue ball is:\n");
	for(i = 0; i < b; i++)
		printf("blue_ball[%d] = {%d, %d}\n", i, blue_ball[i].color, blue_ball[i].weight);

	printf("---------------------------------------------------\n");
	printf("the green ball is:\n");
	for(i = 0; i < g; i++)
		printf("green_ball[%d] = {%d, %d}\n", i, green_ball[i].color, green_ball[i].weight);
	printf("---------------------------------------------------\n");
	printf("After sort, the array is:\n");
	for(i = 0; i < len; i++)
		printf("arr[%d] = {%d, %d}\n", i, arr[i].color, arr[i].weight);
			 

	return arr;
}


int main()
{
	int i;

	printf("Before sort, the array is:\n");
	for(i = 0; i < SIZE; i++)
		printf("arr[%d] = {%d, %d}\n", i, arr[i].color, arr[i].weight);
	printf("---------------------------------------------------\n");

	ball_sort(arr, SIZE);

	return 0;
}


