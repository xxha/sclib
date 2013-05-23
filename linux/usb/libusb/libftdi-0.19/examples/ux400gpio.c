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
#include <semaphore.h>
#include <ftdi.h>

#define UX400_SEM_CPLD  "UX400_SEM_CPLD"
#define	UX400VENDOR	0x0403
#define	UX400PRODUCT	0x6010
#define	UX400DES	"USB <-> Serial Converter"

#define BUF_SIZE 0x10

#define MAX_DEVICES		5
#define TEST_BUFFER		32

#define INIT_CYCLE              50

#define	FAN_SLEEP		20

//#define		MODGPIO_TEST	1

#define	UX400_LOCAL_SN		"USBLOCALBUS01"

#define	REG_GPIO_OFFSET		0x03
#define	REG_CPLDVER		0x15


struct ftdi_context ux400_ftdic;

int Read_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);
int Write_bus(struct ftdi_context * handle, unsigned char haddr, unsigned char laddr, unsigned char * buff, unsigned int len);

int sys_init();


int gpio_set(unsigned char gpio)
{	
	int ret = 0;

        if(( ret = Write_bus(&ux400_ftdic, 0x00, REG_GPIO_OFFSET, &gpio, 1)) < 0)
	{
		printf("Write GPIO reg failed!\n");
		return -1;
	}

	return 0;
}

int gpio_get(unsigned char * gpio)
{	
	int ret = 0;

        if(( ret = Read_bus(&ux400_ftdic, 0x00, REG_GPIO_OFFSET, gpio, 1)) < 0)
	{
		printf("Read GPIO reg failed!\n");
		return -1;
	}

	return 0;
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

	if((ret = ftdi_usb_open_desc(&ux400_ftdic, UX400VENDOR, UX400PRODUCT, NULL, UX400_LOCAL_SN)) < 0)
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


int main(int argc, char *argv[] )
{
	unsigned char fan = 0;
	unsigned char temp = 0;
	unsigned char power = 0;
	int ret = 0;
	int onoff = 0;
	sem_t * sem_id;

	if(argc != 3){
		printf("usage: ux400gpio slot 1/0\n");
		printf("	 slot = LA/LB/LC/RA/RB/RC\n");
		printf("	 1 = on, 0 = off\n");
		exit(0);
	}

	if((ret = sys_init())<0){
		printf("LIBFTDI init failed, exit\n");
		exit(1);
	}

	onoff = atoi(argv[2]);

	if( strcmp(argv[1], "LA") == 0){
		power = 0x04;
	}else if( strcmp(argv[1], "LB") == 0){
		power = 0x02;
	}else if( strcmp(argv[1], "LC") == 0){
		power = 0x01;
	}else if( strcmp(argv[1], "RA") == 0){
		power = 0x10;
	}else if( strcmp(argv[1], "RB") == 0){
		power = 0x08;
	}else if( strcmp(argv[1], "RC") == 0){
		power = 0x20;
	}else{
		printf("Unknow slot, usage: ux400gpio LA/LB/LC/RA/RB/RC 1/0\n", temp);
		exit(0);
	}

        sem_id = sem_open(UX400_SEM_CPLD, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
        if(sem_id == SEM_FAILED) {
                perror("UX400 buzzer sem_open");
                exit(1);
        }

        if(sem_wait(sem_id) < 0) {
                perror("UX400 buzzer sem_wait");
                exit(1);
        }

	if((ret = gpio_get(&temp))<0){
		printf("Read gpio failed, exit\n");
		exit(1);
	}

        if(sem_post(sem_id) < 0) {
                perror("UX400 buzzer sem_post");
        }

	if(onoff == 0){
		temp &= ~power;
	}else{
		temp |= power;
	}

//	printf("GPIO write: 0x%x, onoff: %d, power: 0x%x\n", temp, onoff, power);
        if(sem_wait(sem_id) < 0) {
                perror("UX400 buzzer sem_wait");
                exit(1);
        }

	if((ret = gpio_set(temp))<0){
		printf("Read gpio failed, exit\n");
		exit(1);
	}

        if(sem_post(sem_id) < 0) {
                perror("UX400 buzzer sem_post");
        }

	ftdi_usb_close(&ux400_ftdic);
	ftdi_deinit(&ux400_ftdic);
}




