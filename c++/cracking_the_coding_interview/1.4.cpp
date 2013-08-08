/*
 *    Write a method to decide if two strings are anagrams or not.
 *
 *    An anagram is a word or phrase formed by changing the order of the 
 *    letters in another word or phrase. 
 *    For example, 'triangle' is an anagram of 'integral'
*/

#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;


/*
 *    sort first, then compare.
 *    complexity is O(nlogn).
*/
bool isAnagram1(string s, string t)
{
	if(s == "" || t == "")
		return false;

	if(s.length() != t.length())
		return false;

	sort(&s[0], &s[0] + s.length());
	sort(&t[0], &t[0] + t.length());

	if(s == t)
		return true;
	else
		return false;
}


/*
 *    use array to count the same char times, ++ and -- to zero.
*/

bool isAnagram2(string s, string t)
{
	if(s == "" || t == "")
		return false;
	if(s.length() != t.length())
		return false;

	int len = s.length();
	int c[256];
	memset(c, 0, sizeof(c));

	for(int i = 0; i < len; ++i) {
		++c[(int)s[i]];
		--c[(int)t[i]];
	}
		
	for(int i=0; i<256; ++i)
		if(c[i] != 0)
			return false;

	return true;
}

int main()
{
	string s = "aaabbb";
	string t = "ababab";
	string c = "baababe";

	cout<<isAnagram1(s, t)<<" "<<isAnagram2(s, t)<<endl;
	cout<<isAnagram1(s, c)<<" "<<isAnagram2(s, c)<<endl;

	return 0;
}

