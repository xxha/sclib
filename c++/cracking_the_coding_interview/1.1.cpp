/*
 *   Implement an algorithm to determine if a string has all unique characters. 
 *   What if you can not use additional data structures?
 */
#include <iostream>
#include <cstring>

using namespace std;

/*
 *   create 256 charactor pool
 *   complexity is O(n).
 */
bool isUnique1(string s)
{
	bool a[256];

	memset(a, 0, sizeof(a));
	int len = s.length();
	for(int i = 0; i < len; ++i) {
		int v = (int)s[i];
		if(a[v])
			return false; //the second time means double charactor.
		a[v] = true; //the first time set to true.
	}
	return true;
}

/*
 *    int means 8x4 = 32 bits, 8 int means 256 bits
 *    so here we use int a[8] to replace 256 charactors
 */
bool isUnique2(string s)
{
	int a[8];

	memset(a, 0, sizeof(a));
	int len = s.length();

	for(int i = 0; i < len; ++i) {
		int v = (int) s[i];
		int idx = v/32, shift=v%32;
		if(a[idx] & (1<<shift)) 
			return false;
		a[idx] |= (1<<shift);
	}
	return true;
}

/*
 *    here for a-z (or A-Z), only 28 charactors.
 *    so an int which has 32 bits is enough.
 */

bool isUnique3(string s)
{
	int check = 0;
	int len = s.length();

	for(int i = 0; i < len; ++i) {
		int v = (int)(s[i] - 'a');
		if(check & (1 << v))
			return false;
		check |= (1<<v);
	}
	return true;
}


int main()
{
	string s1 = "I am Lindell Xu.";
	string s2 = "abcdefghijklmnopqrstuvwxyz1234567890";
	string s3 = "abcdefghijklmnopqrstuvwxyz";


	cout<<"isUnique1: "<<isUnique1(s1)<<" "<<isUnique1(s2)<<endl;
	cout<<"isUnique2: "<<isUnique2(s1)<<" "<<isUnique2(s2)<<endl;
	cout<<"isUnique3: "<<isUnique3(s1)<<" "<<isUnique3(s3)<<endl;

	return 0;
}

