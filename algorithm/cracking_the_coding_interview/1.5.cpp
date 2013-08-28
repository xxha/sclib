/*
 * Write a method to replace all spaces in a string with бо%20.
*/

#include <iostream>
#include <cstring>

using namespace std;

/* from start to end */
char* replace1(char *c)
{
	if (c == NULL)
		return NULL;
	int len = strlen(c);

	if(len == 0)
		return NULL;

	int cnt = 0;
	for(int i=0; i<len; ++i) {
		if(c[i] == ' ')
			++cnt;
	}

	char *cc = new char[len + 2*cnt + 1];
	int p = 0;
	for(int i=0; i<len; ++i) {
		if(c[i] == ' ') {
			cc[p] = '%';
			cc[p+1] = '2';
			cc[p+2] = '0';
			p += 3;
		} else {

			cc[p] = c[i];
			++p;
		}
	}

	cc[p] = '\0';
	return cc;
}


/* from end to start */
void replace2(char *c)
{
	if(c == NULL)
		return;
	int len = strlen(c);
	if(len == 0)
		return;

	int cnt = 0;
	for(int i=0; i<len; ++i) {
		if(c[i] == ' ')
			++cnt;
	}

	int p = len + 2*cnt;
	c[p--] = '\0'; //the space mustbe allocated first

	for(int i=len-1; i>=0; --i) {
		if(c[i] == ' ') {
			c[p] = '0';
			c[p-1] = '2';
			c[p-2] = '%';
			p -= 3;
		} else {
			c[p] = c[i];
			--p;
		}
	}
}

int main()
{
	const int len = 100;
	char c[len] = " I am Lindell Xu.";
	char d[len] = " I am Lindell Xu.";

	cout<<"old array c is: "<<c<<endl;
	cout<<"new array c is: "<<replace1(c)<<endl;

	cout<<"old array d is: "<<d<<endl;
	replace2(d);
	cout<<"new array d is: "<<d<<endl;

	return 0;
}
















