#include <stdio.h>

void put1(const char *string)
{
	while(*string != '\0')
		putchar(*string++);
}

int put2(const char *string)
{
	int count = 0;
	while(*string)
	{
		putchar(*string++);
		count++;
	}

	return count;
}

int main(void)
{
	put1("If I had as much money");
	put1("as I could spend, \n");

	printf("I count %d characters.\n",
		put2("I never would cry old chairs to mend.\n"));

	return 0;
}
