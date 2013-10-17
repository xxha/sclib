#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern void log(const char * tag, const char* fmt, ...);
static char logBuffer[2048];

void log(const char * tag, const char* fmt, ...)
{

	char* buf = logBuffer;
	int   pos = 0;

	va_list arglist;
	memset( logBuffer, 0, 1024 );
	
	pos = sprintf(buf, tag);
	buf += pos;
	
	va_start( arglist, fmt );
	pos = vsprintf ( buf,fmt,arglist);
	va_end( arglist );

	buf += pos;
	*buf ++ = 0x0d;
	*buf ++ = 0x0a;
	*buf ++ = 0x00;

	printf( "[%s]: %s",tag,logBuffer );

}

int main()
{
	char *array1 = "hello, ";
	char *array2 = "no, you are very hamesome";
	char *array3 = "3333, hamesome";

	log(array1, array2, array3);

	return 0;
}
