#include <stdio.h>
#include <string.h>


char *strstr( const char *s1, const char *s2 )
{
	int len2;

	if ( !(len2 = strlen(s2)) )
		return (char *)s1;

	for ( ; *s1; ++s1 ) {
		if ( *s1 == *s2 && strncmp( s1, s2, len2 ) == 0 )
		return (char *)s1;
	}

	return NULL;
}

int main()
{
	char *lstr = "hello, how are you?";
	char *sstr = "are";
	char *result;

	result = strstr(lstr, sstr);
	printf("result = %s\n", result);

	return 0;
}
 
