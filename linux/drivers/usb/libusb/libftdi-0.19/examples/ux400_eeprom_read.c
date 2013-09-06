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

#define	REG_CPLDVER		0x15

//#define	UX400_BUS_EEPROM	1


struct ftdi_context ux400_ftdic;

int Read_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);
int Write_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);

int sys_init();



char cpldver(void)
{
	unsigned char data;
	int ret = 0;

	if((ret = Read_bus(&ux400_ftdic, 0x00, REG_CPLDVER, &data, 1) < 0))
	{
		printf("Read CPLD version failed.\n");
		return -1;
	}else{
		printf("CPLD VERSION: 0x%x\n", data);
	}

	return data;
}

int Read_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len)
{
	unsigned char cmd [4] = { 0x00 };
	int ret = 0;

	cmd[0] = 0x91;
	cmd[1] = haddr;
	cmd[2] = laddr;
	cmd[3] = 0x87;

	if((ret = ftdi_usb_purge_rx_buffer (handle)) < 0)
	{
	    fprintf(stderr, "ftdi_usb_purge_rx_buffer failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	if((ret = ftdi_usb_purge_tx_buffer (handle)) < 0)
	{
	    fprintf(stderr, "ftdi_usb_purge_tx_buffer failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	if((ret = ftdi_write_data (handle, cmd, 4)) < 0)
	{
	    fprintf(stderr, "ftdi_write_data failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	if((ret = ftdi_read_data (handle, buff, len)) < 0)
	{
	    fprintf(stderr, "ftdi_read_data failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	return ret;
}

int Write_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len)
{
	unsigned char cmd [4] = { 0x00 };
	int ret = 0;
	int i = 0;

	if((ret = ftdi_usb_purge_tx_buffer (handle)) < 0)
	{
	    fprintf(stderr, "ftdi_usb_purge_tx_buffer failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	for(i = 0; i < len; i ++)
	{
		cmd[0] = 0x93;
		cmd[1] = haddr;
		cmd[2] = laddr;
		cmd[3] = buff[i];

		if((ret = ftdi_write_data (handle, cmd, 4)) < 0)
		{
		    fprintf(stderr, "ftdi_write_data failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		    return EXIT_FAILURE;
		}
	}

	return ret;
}


int sys_init()
{
	int ret, i;
	struct ftdi_device_list *devlist, *curdev;
	char manufacturer[128], description[128], serialno[128];

	if (ftdi_init(&ux400_ftdic) < 0)
	{
		fprintf(stderr, "ftdi_init failed\n");
		return EXIT_FAILURE;
	}

#if 0
	if ((ret = ftdi_usb_find_all(&ux400_ftdic, &devlist, UX400VENDOR, UX400PRODUCT)) < 0)
	{
		fprintf(stderr, "ftdi_usb_find_all failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		return EXIT_FAILURE;
	}

	printf("Number of FTDI devices found: %d\n", ret);

	i = 0;
	for (curdev = devlist; curdev != NULL; i++)
	{
		printf("Checking device: %d\n", i);
		if ((ret = ftdi_usb_get_strings(&ux400_ftdic, curdev->dev, manufacturer, 128, description, 128, serialno, 128)) < 0)
		{
		    fprintf(stderr, "ftdi_usb_get_strings failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
		    return EXIT_FAILURE;
		}
		printf("Manufacturer: %s, Description: %s, Serial number: %s\n\n", manufacturer, description, serialno);
		curdev = curdev->next;
	}

	ftdi_list_free(&devlist);
#endif

#ifdef UX400_BUS_EEPROM
	if((ret = ftdi_usb_open_desc(&ux400_ftdic, UX400VENDOR, UX400PRODUCT, UX400DES, UX400_LOCAL_SN)) < 0)
#else
	if((ret = ftdi_usb_open_desc(&ux400_ftdic, UX400VENDOR, UX400PRODUCT, UX400DES, UX400_RS232_SN)) < 0)
#endif
	{
	    fprintf(stderr, "ftdi_usb_open_desc failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}
	if((ret = ftdi_usb_reset(&ux400_ftdic)) < 0)
	{
	    fprintf(stderr, "ftdi_usb_reset failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	if((ret = ftdi_set_baudrate(&ux400_ftdic, 9600)) < 0)
	{
	    fprintf(stderr, "ftdi_set_baudrate failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	if((ret = ftdi_set_bitmode(&ux400_ftdic, 0x00, BITMODE_RESET)) < 0)
	{
	    fprintf(stderr, "ftdi_set_bitmode failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	usleep(10000);

	if((ret = ftdi_set_bitmode(&ux400_ftdic, 0x00, BITMODE_MCU)) < 0)
	{
	    fprintf(stderr, "ftdi_set_bitmode failed: %d (%s)\n", ret, ftdi_get_error_string(&ux400_ftdic));
	    return EXIT_FAILURE;
	}

	return 0;
}

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

	if((ret = sys_init())<0)
	{
		printf("LIBFTDI init failed, exit\n");
		exit(1);
	}

#ifdef UX400_BUS_EEPROM
	temp = cpldver();
	if(temp < 0 )
	{
		printf("Read CPLD version failed, exit\n");
		exit(1);
	}
#endif

	temp = ftdi_read_eeprom(&ux400_ftdic, eeprom);
	if(temp < 0 )
	{
		printf("Read EEPROM failed, exit\n");
		exit(1);
	}

	for(i = 0; i < EEPROM_SIZE; i ++)
	{
		printf("0x%02x ", eeprom[i]);
	}

	printf("\n");

#ifdef UX400_BUS_EEPROM
	fd = fopen(EEPROM_BUS_FILE_NAME, "w+");
#else
	fd = fopen(EEPROM_RS232_FILE_NAME, "w+");
#endif
	ret = fwrite((void *)eeprom, 1, EEPROM_SIZE, fd);
	if( ret != EEPROM_SIZE){
		printf("fwrite failed! ret=%d\n", ret);
	}

	fclose(fd);

}




