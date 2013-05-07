#include <stdio.h>
#include <string.h>

#define LISTSIZE 5

int main()
{
	char *list[LISTSIZE] = {
		"astronomy", "astounding",
		"astrophysics", "ostrocize"
		"asterism"};

	int count = 0;
	int i;

	for(i = 0; i < LISTSIZE; i++) {
		if(strncmp(list[i], "astro", 5) == 0) {   //比较前n个字符
			printf("Found: %s\n", list[i]);
			count++;
		}
		printf("for over!\n");
	}

	printf("The list contained %d words beginning with astro.\n", count);
	return 0;
}

