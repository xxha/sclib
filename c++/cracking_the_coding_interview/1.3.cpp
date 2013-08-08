/*
 *    Design an algorithm and write code to remove the duplicate characters 
 *    in a string without using any additional buffer. 
 *    NOTE: One or two additional variables are fine. 
 *    An extra copy of the array is not.
 *
 *    FOLLOW UP
 *    Write the test cases for this method.
 */

#include <iostream>
#include <cstring>

using namespace std;

/*
 * here is for a-z only.
 * the complexity is O(n)
*/
string removeDuplicate1(string s)
{
	int check = 0;
	int len = s.length();

	if(len < 2)
		return s;

	string str = "";

	for(int i=0; i<len; ++i) {
		int v = (int)(s[i] - 'a');
		if((check & (1 << v)) == 0) {
			str += s[i]; //string can plus like this way?
			check |= (1<<v);
		}
	}

	return str;
}

/*
 * complexity is O(n2)
*/
string removeDuplicate2(string s)
{
	int len = s.length();
	if(len < 2)
		return s;

	string str = "";

	for(int i=0; i<len; ++i) {
		if(s[i] != '\0') {
			str += s[i];
			for(int j=i+1; j < len; ++j)
				if(s[j] == s[i])
					s[j] = '\0';
		}
	}

	return str;
}


/*
 * comlexity is O(n2)
*/
void removeDuplicate3(char s[])
{
	int len = strlen(s);
	if(len < 2)
		return;

	int p = 0;
	for (int i=0; i<len; ++i) {
		if(s[i] != '\0') {
			s[p++] = s[i];
			for (int j=i+1; j<len; ++j) {
				if(s[j] == s[i])
					s[j] = '\0';
			}
		}
	}
	s[p] ='\0';
}


/*
 * if we could use array.
 * complexity is O(n)
*/
void removeDuplicate4(char s[])
{
	int len = strlen(s);
	if(len < 2)
		return;

	bool c[256];
	memset(c, 0, sizeof(c));
	int p = 0;

	for(int i=0; i<len; ++i) {
		if(!c[s[i]]) {
			s[p++] = s[i];
			c[s[i]] = true;
		}
	}
	s[p] = '\0';
}

void removeDuplicate5(char s[])
{
	int len = strlen(s);

	if(len<2)
		return;

	int check = 0, p = 0;
	for(int i=0; i<len; ++i) {
		int v = (int)(s[i] - 'a');
		if((check & (1<<v)) == 0) {
			s[p++] = s[i];
			check |= (1<<v);
		}
	}
	s[p] = '\0';
}

int main()
{
	string s1 = "abcde";
	string s2 = "aaabbb";
	string s3 = "abababc";
	string s4 = "cccccdbecdeeffbe";

	cout<<removeDuplicate1(s1)<<" "<<removeDuplicate1(s2)<<" "<<removeDuplicate1(s3)<<" "<<removeDuplicate1(s4)<<endl;
	cout<<removeDuplicate2(s1)<<" "<<removeDuplicate2(s2)<<" "<<removeDuplicate2(s3)<<" "<<removeDuplicate2(s4)<<endl;
	char ss1[] = "bacde";
	char ss2[] = "aaabbb";
	char ss3[] = "abababc";
	char ss4[] = "ccccdbecdeeffbe";

	removeDuplicate5(ss1);
	removeDuplicate5(ss2);
	removeDuplicate5(ss3);
	removeDuplicate5(ss4);

	cout<<ss1<<" "<<ss2<<" "<<ss3<<" "<<ss4<<endl;

}

