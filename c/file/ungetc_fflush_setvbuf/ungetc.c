#include <stdio.h>
#include <ctype.h>

void main( void )
{
	int ch;
	int result = 0;

	printf( "Enter an integer: \n" );

	/* Read in and convert number: */
	while( ((ch = getchar()) != EOF) && isdigit( ch ) )  // isdigit(ch) return 0 when ch is not a digit.
		result = result * 10 + ch - '0'; /* Use to format digit. */

	if( ch != EOF )
		ungetc(ch, stdin ); /* Put nondigit back. */

	printf( "Number = %d\nNextcharacter in stream = '%c'\n",
		result, getchar() );
}
