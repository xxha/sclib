#include <stdlib.h>
#include <stdio.h>
int main()
{
	char a[] = "-100" ;
	char b[] = "123" ;
	int c ;

	c = atoi( a ) + atoi( b ) ;
	printf("c = %d\n", c) ;
	
	return 0;
}
