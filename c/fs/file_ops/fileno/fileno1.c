#include <stdio.h>

int main( void )
{
	printf( "The file descriptor for stdin is %d\n", fileno( stdin ) );
	printf( "The file descriptor for stdout is %d\n", fileno( stdout ) );
	printf( "The file descriptor for stderr is %d\n", fileno( stderr ) );

	return 0;
}
