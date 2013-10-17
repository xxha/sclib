/*
 * This program shows how to location an struct array by pointer.
 * 
*/

#include <stdio.h>
#include <string.h>


struct student {
	char name[8];
	int hight;
	int weight;
};

struct student class[5] = {

	{"you",160, 70},
	{"he", 180, 80},
	{"me", 150, 50},
	{"alon", 140, 60},
	{"fat", 190, 90}
};

int main()
{
	int i;
	printf("old struct array is:\n");
	printf("-------------------------------------------------\n");
	for(i=0; i<5; i++)
		printf("	%8s, %4d, %4d\n", class[i].name, class[i].hight, class[i].weight);
	printf("-------------------------------------------------\n");

	printf("1: %s\n", (char *)(class+1));
	printf("1: %s\n", (char *)(class+5));

	printf("1: %s\n", ((struct student *)(class+3))->name);

	printf("2: %d\n", class[1].hight);
	printf("2: %d\n", class[3].weight);

	memcpy((void *)(class + 4), (void *)(class + 1), sizeof(struct student));
	
	printf("new struct array is:\n");
	printf("-------------------------------------------------\n");
	for(i=0; i<5; i++)
		printf("	%8s, %4d, %4d\n", ((struct student *)(class+i))->name,
							((struct student *)(class+i))->hight,
							((struct student *)(class+i))->weight);
	printf("-------------------------------------------------\n");

	return 0;
}
