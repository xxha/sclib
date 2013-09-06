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
#define	UX400DES	"USB <-> Serial Converter"


#define	UX400_LOCAL_SN		"USBLOCALBUS01"
#define	UX400_RS232_SN		"USB2RS232"

#define	EEPROM_SIZE		128
#define	EEPROM_BUS_FILE_NAME	"usb2local01.bin"
#define	EEPROM_RS232_FILE_NAME	"usb2rs232.bin"

int main(int argc, char *argv[] )
{
	char temp = 0;
	int ret = 0;
	unsigned char eeprom[EEPROM_SIZE];
	int i = 0;
	FILE * fd;
	struct ftdi_device_list *devlist, *curdev;
	char manufacturer[128], description[128], serialno[128];
	struct ftdi_context ux400_ftdic;

	if (ftdi_init(&ux400_ftdic) < 0)
	{
		fprintf(stderr, "ftdi_init failed\n");
		return EXIT_FAILURE;
	}

	if ((ret = ftdi_usb_find_all(&ux400_ftdic, &devlist, UX400VENDOR, UX400PRODUCT)) < 0)
	{
		fprintf(stderr, "ftdi_usb_find_all failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	printf("Number of FTDI devices found: %d\n", ret);

	curdev = devlist;

	if ((ret = ftdi_usb_open_dev(&ux400_ftdic, curdev->dev)) < 0)
	{
		fprintf(stderr, "ftdi_usb_open_dev failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	fd = fopen(EEPROM_BUS_FILE_NAME, "rb");
	ret = fread(eeprom, 1, EEPROM_SIZE, fd);
	if(ret != EEPROM_SIZE)
	{
		printf("READ %s failed!\n", EEPROM_BUS_FILE_NAME);
	}
	fclose(fd);

	if ((ret = ftdi_write_eeprom(&ux400_ftdic, eeprom)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	printf("Write EEPROM for %s is successed!\n", EEPROM_BUS_FILE_NAME);

	curdev = curdev->next;

	if ((ret = ftdi_usb_open_dev(&ux400_ftdic, curdev->dev)) < 0)
	{
		fprintf(stderr, "ftdi_usb_open_dev failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	fd = fopen(EEPROM_RS232_FILE_NAME, "rb");
	ret = fread(eeprom, 1, EEPROM_SIZE, fd);
	if(ret != EEPROM_SIZE)
	{
		printf("READ %s failed!\n", EEPROM_RS232_FILE_NAME);
	}
	fclose(fd);

	if ((ret = ftdi_write_eeprom(&ux400_ftdic, eeprom)) < 0)
	{
		fprintf(stderr, "ftdi_write_eeprom failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	printf("Write EEPROM for %s is successed!\n", EEPROM_RS232_FILE_NAME);

	ftdi_list_free(&devlist);
	ftdi_deinit(&ux400_ftdic);

}




