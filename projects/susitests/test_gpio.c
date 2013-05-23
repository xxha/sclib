/****************************************************************************
* Copyright (c) Advantech Co., Ltd. All Rights Reserved
*
* File Name:
*	demo_gpio.c
*
* Programmers:
*	Neo Lo
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "SUSI.h"


#define	JTAG_TDO	0
#define	JTAG_TMS	5
#define	JTAG_TCK	4
#define	JTAG_TDI	6
#define	GPO3		7

#define tck_delay  1000
// Return 0 if platform infomation is correctly obtained.
// Otherwise, return 1.


// Return 0 on success and 1 on failure.
int get_dir_mask(void)
{
	DWORD TargetPinMask, PinDirMask, InPinDirMask, OutPinDirMask;

	if (SusiIOQueryMask(ESIO_SMASK_PIN_FULL, &TargetPinMask) == FALSE) {
		printf("SusiIOQueryMask() failed\n");
		return 1;
	}
    
	if (SusiIOQueryMask(ESIO_DMASK_DIRECTION, &PinDirMask) == FALSE) {
		printf("SusiIOQueryMask() failed\n");
		return 1;
	}
    
	if (SusiIOQueryMask(ESIO_DMASK_IN, &InPinDirMask) == FALSE) {
		printf("SusiIOQueryMask() failed\n");
		return 1;
	}
    
	if (SusiIOQueryMask(ESIO_DMASK_OUT, &OutPinDirMask) == FALSE) {
		printf("SusiIOQueryMask() failed\n");
		return 1;
	}
    
	else
	{
        printf("Total pins mask             = 0x%08lX\n", TargetPinMask);
        printf("direction mask(0:out, 1:in) = 0x%08lX\n", PinDirMask);
		printf("input pins mask             = 0x%08lX\n", InPinDirMask);
		printf("output pins mask            = 0x%08lX\n", OutPinDirMask);
	}
	return 0;
}

// Return 0 on success and 1 on failure.
int read_pin(void)
{
	int result, data;
	BYTE pin;
	BOOL status;

	printf("Which pin (int 0,1,...10,11,...): \n");
	if (scanf("%i", &data) <= 0)
		return 1;
	pin = (BYTE) data;
	result = SusiIOReadEx(pin, &status);
	if (result == FALSE) {
		printf("SusiIOReadEx() failed\n");
		return 1;
	}
	else
		printf("Status: %s\n", status ? "ONE" : "ZERO");
	return 0;
}

// Return 0 on success and 1 on failure.
int read_pins(void)
{
	int result, data;
	DWORD pins, status;

	printf("Bit mask of the pins (hex):\n");
	if (scanf("%x", &data) <= 0)
		return 1;
	pins = (DWORD) data;
	result = SusiIOReadMultiEx(pins, &status);
	if (result == FALSE) {
		printf("SusiIOReadMultiEx() failed\n");
		return 1;
	}
	else
		printf("Status: 0x%lx\n", status);
	return 0;
}

// Return 0 on success and 1 on failure.
int write_pin(void)
{
	int result, data;
	BYTE pin;
	BOOL boolean;

	printf("Which pin (int 0,1,...10,11,...): \n");
	if (scanf("%i", &data) <= 0)
		return 1;
	pin = (BYTE) data;
	printf("Value (0 or 1): \n");
	if (scanf("%i", &data) <= 0)
		return 1;
	boolean = (data) ? 1 : 0;
	result = SusiIOWriteEx(pin, boolean);
	if (result == FALSE) {
		printf("SusiIOWriteEx() failed\n");
		return 1;
	}
	return 0;
}

// Return 0 on success and 1 on failure.
int write_pins(void)
{
	int result, data;
	DWORD pins, status;

	printf("Bit mask of the pins (hex):\n");
	if (scanf("%x", &data) <= 0)
		return 1;
	pins = (DWORD) data;
	printf("Bit pattern (hex):\n");
	if (scanf("%x", &data) <= 0)
		return 1;
	status = (DWORD) data;
	result = SusiIOWriteMultiEx(pins, status);
	if (result == FALSE) {
		printf("SusiIOWriteMulti() failed\n");
		return 1;
	}
	return 0;
}
// Return	0	on success and 1 on	failure.
int set_pin_direction(void)
{
	BYTE PinNum, IO;
	int result, data;
	 
	printf("Which pin (int 0,1,...10,11,...): \n");
	if (scanf("%i", &data) <= 0)
		return 1;
	PinNum = (BYTE)data;
	printf("direction (input pin :1 output pin:0 ): \n");
	if (scanf("%x", &data) <= 0)
		return 1;
	IO = (BYTE)	data ;
	 
	result=SusiIOSetDirection(PinNum,IO, NULL);
	if (result == FALSE) 
	{
		printf("set_pin_direction() failed\n");
		return 1;
	}
	return 0;
}

// Return	0	on success and 1 on	failure.
int set_pins_direction(void)
{
	int result, data;
	DWORD PinNum, PinDirMask;

	printf("mask of	the pins (hex):\n");
	if (scanf("%x", &data) <= 0)
		return 1;
	PinNum = (DWORD) data;
	printf("PinDirMask(0: output, 1: input) (hex):\n");
	if (scanf("%x", &data) <= 0)
		return 1;
	PinDirMask = (DWORD) data;
	result = SusiIOSetDirectionMulti(PinNum, &PinDirMask);
	if (result == FALSE) 
	{
		printf("SusiIOSetDirectionMulti() failed\n");
		return 1;
	}
	return 0;
}

void initialize_jtag_hardware()
{
	int result;

	result = SusiDllInit();
	if (result == FALSE) {
		printf("SusiDllInit() failed\n");
		exit(1);
	}

	result = SusiIOAvailable();
	if (result == 0) {
		printf("SusiIOAvailable() failed\n");
		SusiDllUnInit();
		exit(1);
	}
	
}

void delay_loop(long count)
{
	while (count != 0L) count--;
}

void close_jtag_hardware()
{
	int result;
	
	result = SusiDllUnInit();
	if (result == FALSE) {
		printf("SusiDllUnInit() failed\n");
		exit(1);
	}

}

int jam_jtag_io(int tms, int tdi, int read_tdo)
{
	int data = 0;
	int tdo = 0;
	int result = 0;


	data = ((tdi ? (0x01<<JTAG_TDI) : 0) | (tms ? (0x01<<JTAG_TMS) : 0));
	printf("data1 = 0x%x\n", data);

	result = SusiIOWriteMultiEx((0x01<<JTAG_TCK)|(0x01<<JTAG_TDI)|(0x01<<JTAG_TMS), data);
	if (result == FALSE) {
		printf("SusiIOWriteMulti() failed\n");
		exit(1);
	}
	if (read_tdo)
	{
		result = SusiIOReadEx(JTAG_TDO, &tdo);
		if (result == FALSE) {
			printf("SusiIOReadEx() failed\n");
			exit(1);
		}
	}
	
	data = (tdi ? (0x01<<JTAG_TDI) : 0) | (tms ? (0x01<<JTAG_TMS) : 0) | (0x01<<JTAG_TCK) ;
	printf("data2 = 0x%x\n", data);

	result = SusiIOWriteMultiEx((0x01<<JTAG_TCK)|(0x01<<JTAG_TDI)|(0x01<<JTAG_TMS), data|(0x01<<JTAG_TCK));
	if (result == FALSE) {
		printf("SusiIOWriteMulti() failed\n");
		exit(1);
	}

	if (tck_delay != 0) delay_loop(tck_delay);

	return (tdo);
}


int main(void)
{
	int result;
	initialize_jtag_hardware();

#if 1
	result = SusiIOWriteMultiEx(7, 1);
	if (result == FALSE) {
		printf("SusiIOWriteEx() failed\n");
		return 1;
	}
#endif
#if 0
	while(1) {
	jam_jtag_io(0, 0, 0);

	}
#endif
	close_jtag_hardware();

	return 0;
}

