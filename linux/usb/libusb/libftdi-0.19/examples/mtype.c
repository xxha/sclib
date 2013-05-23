#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>


#define	UX400_10G	"BVUK"
#define	UX400_100GE	"BVUH"
#define UX400_2P5G	"BVUJ"
#define	UX400_40G	"BVUX"
#define UX400_1GE	"BVUL"
#define UX400_40GE	"BVUM"
#define	UX400_16G	"BVUP"

#define	UX400_LA_PATH	"/sys/bus/usb/devices/1-1.2/serial"
#define	UX400_LB_PATH	"/sys/bus/usb/devices/1-1.3/serial"
#define	UX400_LC_PATH	"/sys/bus/usb/devices/1-1.4/serial"
#define	UX400_RA_PATH	"/sys/bus/usb/devices/1-5/serial"
#define	UX400_RB_PATH	"/sys/bus/usb/devices/1-3/serial"
#define	UX400_RC_PATH	"/sys/bus/usb/devices/1-1.1/serial"


int main(int argc,char *argv[])
{
	unsigned char * bpath = NULL;
	FILE *sfile = NULL;
	unsigned char modtype[64];

	if(argc < 2){
		printf("Usage: ./mtype slot\n");
		exit(0);
	}

	if( strcmp(argv[1], "LA") == 0){
		bpath = UX400_LA_PATH;
	}else if( strcmp(argv[1], "LB") == 0){
		bpath = UX400_LB_PATH;
	}else if( strcmp(argv[1], "LC") == 0){
		bpath = UX400_LC_PATH;
	}else if( strcmp(argv[1], "RA") == 0){
		bpath = UX400_RA_PATH;
	}else if( strcmp(argv[1], "RB") == 0){
		bpath = UX400_RB_PATH;
	}else if( strcmp(argv[1], "RC") == 0){
		bpath = UX400_RC_PATH;
	}else{
		printf("Unknow slot, please make sure you enter LA/LB/LC/RA/RB/RC\n");
		exit(0);
	}

	sfile = fopen(bpath, "r");
	if( sfile == NULL ){
		printf("Can't find any board in slot: %s\n", argv[1]);
		exit(0);
	}

	if( fgets(modtype, sizeof(modtype), sfile) == NULL){
		printf("Can't read serial number of %s\n", bpath);
		exit(0);
	}

	if(sfile != NULL) fclose(sfile);

	if( strncmp(modtype, UX400_10G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_10G, argv[1]);
		exit(1);
	}else if( strncmp(modtype, UX400_100GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_100GE, argv[1]);
		exit(2);
	}else if( strncmp(modtype, UX400_2P5G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_2P5G, argv[1]);
		exit(3);
	}else if( strncmp(modtype, UX400_40G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_40G, argv[1]);
		exit(4);
	}else if( strncmp(modtype, UX400_1GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_1GE, argv[1]);
		exit(5);
	}else if( strncmp(modtype, UX400_40GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_40GE, argv[1]);
		exit(6);
	}else if( strncmp(modtype, UX400_16G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_16G, argv[1]);
		exit(7);
	}else{
		printf("Unknow board serial number: %s in slot: %s\n", modtype, argv[1]);
		exit(0);
	}
}

