#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int my_atoi(const char* p){
	assert(p != NULL);
	bool neg_flag = false;// ....
	int res = 0;// ..
	if(p[0] == '+' || p[0] == '-')
	neg_flag = (*p++ != '+');
	while(isdigit(*p)) res = res*10 + (*p++ - '0');
	return neg_flag ?0 -res : res;
}

int main(void)
{
	int n;
	char *str = "12345.67";
	n = my_atoi(str);
	printf("string = %s integer = %d\n", str, n);
	return 0;
}
