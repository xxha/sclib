#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libeeprom.h"

//#define	SHOW_DATA		1

#define	MAXTEST			0xFF

int main(int argc, char** argv)
{
	int r = 0;
	int i, j;
	unsigned char testbuf[MAXTEST], rxbuf[MAXTEST];
	int testtimes = 0;
	int errors = 0;

	r = eeprom_open();
	if( r < 0 ){
		printf("eeprom open failed!\n");
		exit(1);
	}

//for(;;){
	r = eeprom_read_buf(0x0, MAXTEST, rxbuf);
	if( r < 0 ){
		printf("eeprom read failed!\n");
		exit(1);
	}

	printf("\nThe EEPROM contents:\n      ");
	for(i = 0; i < 16; i ++){
		printf(" %02x  ", i);
	}
	printf("\n[00]: ");

	for(i = 1, j = 1; i <= MAXTEST; i ++){
		printf("0x%02x,", rxbuf[i-1]);
		if(i %16 == 0){
			if(i < MAXTEST) printf("\n[%02x]: ", j++);
			else printf("\n");
		}
	}
	printf("\n");
//}

	eeprom_close();
}


