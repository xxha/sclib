#include<string.h>
#include<stdio.h>
int main(void)
{
	char input[16]="a,b,c,d";
	char *p;

	p=strtok(input,",");

	while(p) {
		printf("%s\n",p);
		p=strtok(NULL,",");
	}

	return 0;
}
