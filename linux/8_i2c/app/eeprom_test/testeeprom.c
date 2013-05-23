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

	if(argc < 2){
		printf("Usage: ./testeeprom testtimes\n");
		exit(1);
	}

	testtimes = atoi(argv[1]);
	printf("testtimes: %d\n", testtimes);

	r = eeprom_open();
	if( r < 0 ){
		printf("eeprom open failed!\n");
		exit(1);
	}

	for(i = 0; i < MAXTEST; i ++){
		testbuf[i] = i;
	}

	if(argc >= 2){
		printf("We write 0x00 - 0xFE to the EEPROM.\n");
		r = eeprom_write_buf(0x0, sizeof(testbuf), testbuf);
		if( r < 0 ){
			printf("eeprom write failed!\n");
			exit(1);
		}

		r = eeprom_read_buf(0x0, MAXTEST, rxbuf);
		if( r < 0 ){
			printf("eeprom read failed!\n");
			exit(1);
		}

		printf("\nNow, we read it back\n");
		for(i = 0; i < MAXTEST; i ++){
			printf("0x%02x,", rxbuf[i]);
			if(i %15 == 0) printf("\n");
		}
		printf("\n");
	}

	printf("\nWe start read test now!\n");

	for(j = 0; j < testtimes; j ++){
		r = eeprom_read_buf(0x0, MAXTEST, rxbuf);
		if( r < 0 ){
			printf("eeprom read failed!\n");
			exit(1);
		}
	
		for(i = 0; i < MAXTEST; i ++){
			if(rxbuf[i] != testbuf[i]){
				errors ++;
				printf("\nrxbuf[%d]=0x%x\n", rxbuf[i]);
			}
		}

		memset(rxbuf, 0, MAXTEST);
	}
	
	if(errors > 0) printf("\nEEPROM operation failed [%d] times in [%d] times.\n", errors, testtimes);
	else	printf("\nEEPROM read successed [%d] times!\n", testtimes);

	eeprom_close();
}


