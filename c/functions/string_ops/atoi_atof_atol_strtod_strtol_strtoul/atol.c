#include<stdlib.h>
#include<stdio.h>

main()
{
	char a[]="1000000000";
	char b[]="234567890";
	long c;

	printf("atol(a) = %ld\n",atol(a));
	printf("atol(b) = %ld\n",atol(b));

	c=atol(a)+atol(b);
	printf("c=%ld\n",c);

	return 0;
}
