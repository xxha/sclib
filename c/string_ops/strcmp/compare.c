#include <stdio.h>
#include <string.h>

#define ANSWER "Grant"
#define MAX 40

int main(void)
{
	char try[MAX];

	puts("Who is buried in Grant's tomb?");
	gets(try);

	while (strcmp(try, ANSWER) != 0) {                  //如果两字符相同，则返回 0, 不同时，返回非0
		puts("No, that's wrong. Try again.");
		gets(try);

	}
	puts("That's right!");
	return 0;
}
