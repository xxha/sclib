#include <stdio.h>
#include <string.h>

int main()
{
	int len, nel;
	char query[] ="user_command=appleboy&test=1&test2=2";
	char *q, *name, *value;

	/* Parse into individualassignments */
	q = query;
	fprintf(stderr, "CGI[query string] : %s\n",query);
	len = strlen(query);
	nel = 1;

	while (strsep(&q, "&"))
		nel++;

	fprintf(stderr, "CGI[nel string] : %d\n", nel);
	for (q = query; q< (query + len);) {
		value = name = q;

		/* Skip to next assignment */
		fprintf(stderr, "CGI[string] :%s\n", q);
		fprintf(stderr, "CGI[stringlen] : %d\n", strlen(q));
		fprintf(stderr, "CGI[address] :%x\n", q);

		for (q += strlen(q); q < (query +len) && !*q; q++);
			/* Assign variable */
			name = strsep(&value,"=");

		fprintf(stderr, "CGI[name ] :%s\n", name);
		fprintf(stderr, "CGI[value] :%s\n", value);
	}
	return 0;
}

