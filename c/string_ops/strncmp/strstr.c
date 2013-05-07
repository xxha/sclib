#include <stdio.h>
#include <string.h>

char *strofstr(char *lstr, char *sstr);

char *strofstr(char *lstr, char *sstr)
{
	char *backup;

	backup = lstr;

	while(*lstr) {
		if(!strncmp(lstr, sstr, strlen(sstr))) {
			printf("Found \"%s\" in \"%s\".\n", sstr, lstr);
			return lstr;
		}
		lstr++;
	}
	printf("no \"%s\" found in \"%s\".\n", sstr, backup);
	return NULL;
}

int main()
{
	char *longstr = "astronomy";
	char *shortstr = "tron";
	char *ret;
	
	ret = strofstr(longstr, shortstr);

	printf("The return is: %s \n", ret);
	return -1;
}


