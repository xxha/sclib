/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <ftdi.h>


#define	UX400VENDOR	0x0403
#define	UX400PRODUCT	0x6010

#define	UX400_2G5_TESTSN	"BVUXA01KA610"

#define	EEPROM_SIZE		128

#define	OPTION_OFFSET_FTDI	0x70
#define	OPTION_SIZE_FTDI	14


struct ftdi_context ux400_ftdic;
unsigned char eeprom[EEPROM_SIZE];

unsigned short ft2232_checksum(unsigned char * eeprom, int size);
int ftdi_write_eeprom_bytes( struct ftdi_context * ftdi, unsigned char addr, int len);

int read_options(unsigned char * options);
int write_options(unsigned char * options);
int sys_init();


int read_options(unsigned char * options)
{
	int i;
	int ret;
	unsigned short checksum;
	unsigned short oldchecksum;

	if ((ret = ftdi_read_eeprom(&ux400_ftdic, eeprom)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	checksum = ft2232_checksum(eeprom, EEPROM_SIZE);
	oldchecksum = (eeprom[EEPROM_SIZE-1] << 8) | eeprom[EEPROM_SIZE-2];
	
	if(checksum != oldchecksum){
		printf("Checksum error, old: 0x%x, new: 0x%x\n", oldchecksum, checksum);
		return -1;
	}

	for(i = 0; i < OPTION_SIZE_FTDI; i ++){
		if(options != NULL) options[i] = eeprom[OPTION_OFFSET_FTDI+i];
		else	return -1;
	}
	
	return 0;
}

int write_options(unsigned char * options)
{
	int i;
	int ret;
	unsigned short checksum;
	unsigned short oldchecksum;

	if(options == NULL) return -1;
	
	if ((ret = ftdi_read_eeprom(&ux400_ftdic, eeprom)) < 0)
	{
		fprintf(stderr, "ftdi_read_eeprom failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	checksum = ft2232_checksum(eeprom, EEPROM_SIZE);
	oldchecksum = (eeprom[EEPROM_SIZE-2] << 8) | eeprom[EEPROM_SIZE-1];

	for(i = 0; i < OPTION_SIZE_FTDI; i ++){
		eeprom[OPTION_OFFSET_FTDI+i] = options[i];
	}

	checksum = ft2232_checksum(eeprom, EEPROM_SIZE);
	printf("The checksum: 0x%x\n", checksum);

	eeprom[EEPROM_SIZE-2] = checksum;
	eeprom[EEPROM_SIZE-1] = checksum >> 8;

#if 0
	if ((ret = ftdi_write_eeprom_bytes(&ux400_ftdic, eeprom+OPTION_OFFSET_FTDI, OPTION_SIZE_FTDI)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom_bytes failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	if ((ret = ftdi_write_eeprom_bytes(&ux400_ftdic, eeprom+EEPROM_SIZE-2, 2)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom_bytes failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}
#endif

	if ((ret = ftdi_write_eeprom(&ux400_ftdic, eeprom)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	return 0;
}

int ftdi_write_eeprom_bytes( struct ftdi_context * ftdi, unsigned char addr, int len)
{
	unsigned short usb_val;
	int i;
	
	for(i = addr/2; i < len/2; i ++){
		usb_val = eeprom[i*2];
		usb_val = eeprom[(i*2)+1] << 8;
		if(usb_control_msg(ftdi->usb_dev, 0x40, 0x91, usb_val, i, NULL, 0, ftdi->usb_write_timeout) != 0){
			return -1;
		}
	}
	
	return 0;
}


unsigned short ft2232_checksum(unsigned char * eeprom, int size)
{
	unsigned short checksum, value;
	int i;
	
	checksum = 0xAAAA;
	
	for(i = 0; i < size/2-1; i ++){
		value = eeprom[i*2];
		value += eeprom[(i*2)+1] << 8;
		
		checksum = value^checksum;
		checksum = (checksum << 1)|(checksum >> 15);
	}
	
	return checksum;
}

int sys_init(unsigned char * sn)
{
	int ret;
	
	if (ftdi_init(&ux400_ftdic) < 0)
	{
		fprintf(stderr, "ftdi_init failed\n");
		return EXIT_FAILURE;
	}

	if((ret = ftdi_usb_open_desc(&ux400_ftdic, UX400VENDOR, UX400PRODUCT, NULL, sn)) < 0)
	{
	    fprintf(stderr, "ftdi_usb_open_desc failed: %d (%s), sn: %s\n", ret, ftdi_get_error_string(&ux400_ftdic), sn);
	    return EXIT_FAILURE;
	}
	
	return 0;
}

int main(int argc, char *argv[] )
{
	int ret = 0;
	int i = 0;
	unsigned char temp[OPTION_SIZE_FTDI];

	ret = sys_init(UX400_2G5_TESTSN);
	if( ret != 0 ){
		printf("FTDI init failed for serial number: %s\n", UX400_2G5_TESTSN);
		exit(1);
	}

	ret = read_options( temp );
	if( ret != 0 ){
		printf("FTDI read options failed for serial number: %s\n", UX400_2G5_TESTSN);
		ftdi_deinit(&ux400_ftdic);
		exit(1);
	}
	
	printf("Options: \n");
	for(i = 0; i < OPTION_SIZE_FTDI; i ++){
		printf("0x%x ", temp[i]);
	}
	printf("\n");

#if 0	// options write test.
	for(i = 0; i < OPTION_SIZE_FTDI; i ++){
		temp[i] = i;
	}
		
	ret = write_options( temp );
	if( ret != 0 ){
		printf("FTDI write options failed for serial number: %s\n", UX400_2G5_TESTSN);
		ftdi_deinit(&ux400_ftdic);
		exit(1);
	}

	ret = read_options( temp );
	if( ret != 0 ){
		printf("FTDI read options failed for serial number: %s\n", UX400_2G5_TESTSN);
		ftdi_deinit(&ux400_ftdic);
		exit(1);
	}
	
	printf("The new options: \n");
	for(i = 0; i < OPTION_SIZE_FTDI; i ++){
		printf("0x%x ", temp[i]);
	}
	printf("\n");
#endif

	ftdi_deinit(&ux400_ftdic);

}


