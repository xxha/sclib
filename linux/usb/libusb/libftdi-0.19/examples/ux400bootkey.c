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

#include <sys/wait.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define	UX400VENDOR	0x0403
#define	UX400PRODUCT	0x6010
#define	UX400DES	"USB <-> Serial Converter"

#define BUF_SIZE 0x10

#define MAX_DEVICES		5
#define TEST_BUFFER		32


#define	POWERKEY		0x01

#define	BUZZER_DELAY		250000

#define	UX400_LOCAL_SN		"USBLOCALBUS01"

#define	BACKLIGHT_OFFSET	0x01
#define	POWERKEY_OFFSET		0x08
#define	KEYOFFSET		0x0A

#define	REG1_OFFSET		0x00
#define	REG_GPIO_OFFSET		0x03
#define	REG_CPLDVER		0x15

#define	VEEX_KEY_BACKLIGHT	0x1A
#define	VEEX_KEY_SUMARY		0x16
#define	VEEX_KEY_HELP		0x0E
#define	VEEX_KEY_FILE		0x19
#define	VEEX_KEY_HISTORY	0x15

#define	UX400_SEM_CPLD	"UX400_SEM_CPLD"


struct ftdi_context ux400_ftdic;

int Read_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);
int Write_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);

int sys_init();

inline char keyscan(void)
{
	unsigned char data;
	unsigned int ret;
	sem_t * sem_id;

	sem_id = sem_open(UX400_SEM_CPLD, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
	if(sem_id == SEM_FAILED) {
		perror("UX400 KEY sem_open");
		exit(1);
	}

	if(sem_wait(sem_id) < 0) {
		perror("UX400 KEY sem_wait");
		exit(1);
	}

	ret = Read_bus(&ux400_ftdic, 0x00, KEYOFFSET, &data, 1);

	if(sem_post(sem_id) < 0) {
		perror("UX400 KEY sem_post");
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

	if((ret = ftdi_usb_open_desc(&ux400_ftdic, UX400VENDOR, UX400PRODUCT, UX400DES, UX400_LOCAL_SN)) < 0)
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

int bootkey()
{
	char temp1 = 0;
	char temp2 = 0;

	do{
		temp1 = keyscan();
		temp2 = keyscan();
	}while(temp1 != temp2);
	
	if( temp1 == VEEX_KEY_BACKLIGHT ) return 1;
	else return 0;
}

int main(int argc, char *argv[] )
{
	int ret = 0;
	int i,try=1;
	if((ret = sys_init())<0)
	{
		printf("LIBFTDI init failed, exit\n");
		exit(1);
	}
	for(i=0;i<try;i++){
		ret = bootkey();
		if(1==ret)
			break;
		sleep(1);
	}
	if(ret == 1){ 
		printf("Bootkey press!\n");
		return 0;
	}
	else{
		printf("Can't find boot key press.\n");
		return 1;
	}
}





