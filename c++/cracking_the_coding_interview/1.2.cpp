/* 
 *    Write code to reverse a C-Style String. 
 *    (C-String means that ¡°abcd¡± is represented as five characters
 *    , including the null character.)
*/

#include <iostream>
#include <cstring>

using namespace std;

/*
 * use xor to exchange 2 chars.
*/
void swap(char &a, char &b)
{
	a = a^b;
	b = a^b;
	a = a^b;
}

/* do not care about string ending '\0' */
void reverse2(char *s) 
{
	int n = strlen(s);
	for(int i = 0; i < n/2; ++i)
		swap(s[i], s[n - i - 1]);
}

/* care about string ending '\0'*/
void reverse1(char *s)
{
	if(!s)
		return;
	char *p = s, *q = s;

	while(*q)
		++q;
	--q;
	while(p < q)
		swap(*p++, *q--);
}

int main()
{
	char s1[] = "1234567890abcdefghijklmnopqrstuvwxyz";
	char s2[] = "1234567890abcdefghijklmnopqrstuvwxyz";

	reverse1(s1);
	cout<<s1<<endl;

	reverse2(s2);
	cout<<s2<<endl;

	return 0;
}
