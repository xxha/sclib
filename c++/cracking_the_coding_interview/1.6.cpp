/* 
 * Given an image represented by an NxN matrix, 
 * where each pixel in the image is 4 bytes, 
 * write a method to rotate the image by 90 degrees. 
 * Can you do this in place?
*/

#include <iostream>

using namespace std;

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

/*
 * Two steps to make 90 degree transpose:
 *
 * 1. exchange the data along diagonal.
 * 2. exchange the i line and n-i-1 line.
*/

void transpose(int a[][4], int n)
{
	for (int i=0; i<n; ++i)
		for (int j=i+1; j<n; ++j)
			swap(a[i][j], a[j][i]);

	for (int i=0; i<n/2; ++i)
		for (int j=0; j<n; ++j)
			swap(a[i][j], a[n-1-i][j]);
}

int main()
{
	int a[4][4] = {
	{1, 2, 3, 4},
	{5, 6, 7, 8},
	{9, 10, 11, 12},
	{13, 14, 15, 16}
	};

	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j)
			cout<<a[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;

	transpose(a, 4);
	for(int i=0; i<4; ++i) {
		for(int j=0; j<4; ++j)
			cout<<a[i][j]<<" ";
		cout<<endl;
	}

	return 0;
}
