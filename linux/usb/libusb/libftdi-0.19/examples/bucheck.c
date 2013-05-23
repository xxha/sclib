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


#define	BUF_SIZE	256

int main(int argc,char *argv[])
{
	char * bpath = NULL;
	char * mversion = NULL;
	char * sversion = NULL;
		
	FILE *sfile = NULL;
	FILE *vfile = NULL;
	
	char modtype[BUF_SIZE];
	char verpath[BUF_SIZE];
	char softver[BUF_SIZE];
	
	int i = 0;

	if(argc < 2){
		printf("Usage: ./bucheck slot\n");
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

	mversion = strchrnul(modtype, ':');
	if(mversion[0] != '\0')	mversion += 1;
	
	verpath[0] = '\0';
	
	strcat(verpath, "/usr/local/");
	
	if( strncmp(modtype, UX400_10G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_10G, argv[1]);
		strcat(verpath, "10g");
	}else if( strncmp(modtype, UX400_100GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_100GE, argv[1]);
		strcat(verpath, "100ge");
	}else if( strncmp(modtype, UX400_2P5G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_2P5G, argv[1]);
		strcat(verpath, "2p5g");
	}else if( strncmp(modtype, UX400_40G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_40G, argv[1]);
		strcat(verpath, "40g");
	}else if( strncmp(modtype, UX400_1GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_1GE, argv[1]);
		strcat(verpath, "1ge");
	}else if( strncmp(modtype, UX400_40GE, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_40GE, argv[1]);
		strcat(verpath, "40ge");
	}else if( strncmp(modtype, UX400_16G, 4) == 0){
		printf("We found: %s in slot: %s\n", UX400_16G, argv[1]);
		strcat(verpath, "16g");
		exit(7);
	}else{
		printf("Unknow board serial number: %s in slot: %s\n", modtype, argv[1]);
		exit(0);
	}
	
	strcat(verpath, "/lib/vendor-info.txt");
	
	vfile = fopen(verpath, "r");
	if( vfile == NULL ){
		printf("Can't find any board in slot: %s\n", argv[1]);
		exit(0);
	}

	if( fgets(softver, sizeof(softver), vfile) == NULL){
		printf("Can't read serial number of %s\n", verpath);
		exit(0);
	}

	if(vfile != NULL) fclose(vfile);

	sversion = strchrnul(softver, ':');
	if(sversion[0] != '\0')	sversion += 2;	
	
	printf("module software version: %s", mversion);
	printf("local software version: %s", sversion);
	
	if(strcmp(sversion, mversion) != 0){
		printf("The module: %s on slot: %s should be update!\n", modtype, argv[1]);
		exit(1);
	}else{
		printf("The module include the last software, it doesn't need upgrade at all!\n");
	}
	
	exit(0);
}



