//============================================================================
// Name : debug.c
// Author : boyce
// Version : 1.0
// Copyright : pku
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>

#define __DEBUG__

#ifdef __DEBUG__
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

int main(int argc, char **argv)
{
	char str[] = "Hello World";
	int  test = 10;
	char *mark = "fuck you";

	DEBUG("A ha, check me: %s, test = %d, mark = %s\n", str, test, mark);

	return 0;
}

