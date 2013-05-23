/****************************************************************************
* Copyright (c) Advantech Co., Ltd. All Rights Reserved
*
* File Name:
*	demo_vga.c
*
* Programmers:
*	Neo Lo
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SUSI.h"

// Return 0 if platform infomation is correctly obtained.
// Otherwise, return 1.
int show_platform_info(void)
{
	BOOL result;
	DWORD major, minor, year, month, date;
	DWORD size = 128;
	TCHAR buf[size];  // Buffer length includes the null character.

	SusiDllGetVersion(&major, &minor);
    year    = minor/10000;
    month   = minor%10000/100;
    date    = minor%100;
	printf("Version: %li (20%02li/%02li/%02li)\n", major, year, month, date);

	// Get platform name.
	result = SusiCoreGetPlatformName(buf, &size);
	if (result != TRUE) {
		printf("SusiCoreGetPlatformName() failed\n");
		return 1;
	}
	else
		printf("Platform name: %s\n", buf);

	// Get BIOS version.
	result = SusiCoreGetBIOSVersion(buf, &size);
	if (result != TRUE) {
		return 1;
	}
	else
		printf("BIOS version: %s\n", buf);

	return 0;
}

void show_menu(void)
{
	printf("\n");
	printf("0) Terminate this program\n");
	printf("1) Turn on the screen\n");
	printf("2) Turn off the screen\n");
	printf("Enter your choice: \n");
}

int main(void)
{
	int result;
	int i = 0;
//	int done, op;

REPEAT:
	result = SusiDllInit();
	if (result == FALSE) {
		printf("SusiDllInit() failed\n");
		sleep (1);
		if(i ++ > 10) exit(1);
		goto REPEAT;
	}

	result = SusiVCAvailable();
	if ((result & VC_VGA_CTL_SUPPORT) == 0) {
		printf("SusiVCAvailable() = %d failed\n", result);
		SusiDllUnInit();
		sleep (1);
		if(i ++ > 20) exit(1);
		goto REPEAT;
		
	}

	result = show_platform_info();

	result = SusiVCScreenOff();

	sleep(1);

	result = SusiVCScreenOn();

#if 0
	done = 0;
	while (! done) {
		show_menu();
		if (scanf("%i", &op) <= 0)
			op = -1;

		switch (op) {
		case 0:
			done = 1;
			continue;
		case 1:
			result = SusiVCScreenOn();
			break;
		case 2:
			result = SusiVCScreenOff();
			break;
		default:
			printf("\nUnknown choice!\n\n");
			continue;
		}
		if (result == FALSE) {
			SusiDllUnInit();
			return 1;
		}
	}
#endif

	result = SusiDllUnInit();
	if (result == FALSE) {
		printf("SusiDllUnInit() failed\n");
		return 1;
	}

	return 0;
}

