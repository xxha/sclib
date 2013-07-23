#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int demo( char * msg, ... );

void main( void )
{
	demo("DEMO", "This", "is", "a", "demo!","");
}
	
int demo( char *msg, ...)
{
	va_list argp;
	int argno = 0;
	char *para;

	va_start( argp, msg);
	while (1)
	{
		para = va_arg( argp, int);
		if ( strcmp( para, "") == 0 )
			break;
		printf("Parameter #%d is: %s\n", argno, para);
		argno++;
	}
	va_end( argp );
	
	return 0;
}
