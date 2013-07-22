/* By Yanjun Luo, for MX27 gpio test with application software. */

#include <unistd.h>
#include <stdlib.h>
#include "cpld_gpio.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define msleep(x) usleep(x*1000)

void usage()
{
	printf("cpld_gpio usage:\n");
	printf("\n");
	printf(" 	cpld_gpio eth/otdr gpio_number 1/0\n");
	printf("\n");
	printf(" 	gpio_number: [1-3] for eth, [1-5] for otdr\n");
	printf(" 	1:output high voltage, 0:output low voltage\n");
}


int main(int argc, char *argv[])
{
	int ret = 0;
	unsigned int gpio = 0;
	unsigned char data = 0;
	unsigned char type[5];
	unsigned char *eth = "eth";
	unsigned char *otdr = "otdr";
	
	if (argc != 4) {
		usage();
		return 0;
	}

	ret = gpio_init();
	if(ret < 0){
		printf("GPIO init failed!\n");
		exit(1);
	}

	memset(type, 0, 5);
	strcpy(type, argv[1]);
	printf("type = %s\n", type);

	gpio = atoi(argv[2]);
	data = atoi(argv[3]);
	printf("gpio = %d\n", gpio);
	printf("data = %d\n", data);

	if(!(strcmp(type, otdr))) {
		if(set_otdr_gpio(gpio, data))
			return -1;
		printf("set otdr succeed\n");

		msleep(500);
		get_otdr_gpio(gpio, &data);
		printf("read otdr data = %d\n", data);

	        gpio_exit();
	        return 0;

	}

	if(!(strcmp(type, eth))) {
		if(set_eth_gpio(gpio, data))
			return -1;
		printf("set eth succeed\n");

		msleep(500);
		get_eth_gpio(gpio, &data);
		printf("read eth data = %d\n", data);

	        gpio_exit();
	        return 0;
	}

	printf("nothing set\n");	
	gpio_exit();	
	return -1;
}

