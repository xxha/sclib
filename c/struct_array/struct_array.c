#include <stdio.h>

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
	printf("1: %s\n", (void *)(class+1));
	printf("2: %d\n", (void *)(class+1));

	return 0;
}
