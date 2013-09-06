#include "cpld_gpio.h"

#define CONT_REG3 	0x2
#define CONT_REG4 	0x3

#define CPLD_BASE_ADDR 0xC8000000
#define MAP_SIZE 256UL
#define MAP_MASK (MAP_SIZE - 1)


volatile void *cpld_base = NULL;
int fd = 0;

int gpio_init()
{

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd == -1) {
		printf("Error opening /dev/mem\n");
		return -1;
	}

	cpld_base = mmap((void *)0, MAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, CPLD_BASE_ADDR & ~MAP_MASK);
	if (cpld_base == NULL) {
		printf("Error mapping cpld memory\n");
		close(fd);
		return -1;
	}

	return 0;
}

int gpio_exit()
{
	if (munmap((void *)cpld_base, MAP_SIZE)) {
		printf("Error unmapping cpld memory\n");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

static void set_eth_pin(unsigned int pin, unsigned char data)
{
	if (data) {
		*((unsigned char *)(cpld_base + CONT_REG3)) |= (0x1 << pin);
	} else {
		*((unsigned char *)(cpld_base + CONT_REG3)) &= ~(0x1 << pin);
	}

}

int set_eth_gpio(unsigned int gpio, unsigned char data)
{
	if ((gpio < 0) || (gpio > 3)) {
		printf("Invalid pin number\n");
		return -1;
	}

	switch (gpio) {
		case ETH_GPIO1:
			set_eth_pin(ETH_PIN1, data);
			break;
		case ETH_GPIO2:
			set_eth_pin(ETH_PIN2, data);
			break;
		case ETH_GPIO3:
			set_eth_pin(ETH_PIN3, data);
			break;
		default:
			printf("Invalid pin number\n");
			return -1;
	}

	return 0;
}

int get_eth_gpio (unsigned int gpio, unsigned char *data)
{
	unsigned char eth = 0;

	if ((gpio < 0) ||(gpio > 3)) {
		printf("Invalid pin number\n");
		return -1;	
	}

	eth = *((unsigned char *)(cpld_base + CONT_REG3));

	switch (gpio) {
		case ETH_GPIO1:
			*data = (eth & (0x1 << ETH_PIN1)) ? 1:0;
			break;
		case ETH_GPIO2:
			*data = (eth & (0x1 << ETH_PIN2)) ? 1:0;
			break;
		case ETH_GPIO3:
			*data = (eth & (0x1 << ETH_PIN3)) ? 1:0;
			break;
		default:
			printf("Invalid pin number\n");
			return -1;
	}

	return 0;
}

static void set_otdr_pin(unsigned int pin, unsigned char data)
{
	if (data) {
		*((unsigned char *)(cpld_base + CONT_REG4)) |= (0x1 << pin);
	} else {
		*((unsigned char *)(cpld_base + CONT_REG4)) &= ~(0x1 << pin);
	}

}

int set_otdr_gpio(unsigned int gpio, unsigned char data)
{
	if ((gpio < 0) || (gpio > 5)) {
		printf("Invalid pin number\n");
		return -1;
	}

	switch (gpio) {
		case OTDR_GPIO1:
			set_otdr_pin(OTDR_PIN1, data);
			break;
		case OTDR_GPIO2:
			set_otdr_pin(OTDR_PIN2, data);
			break;
		case OTDR_GPIO3:
			set_otdr_pin(OTDR_PIN3, data);
			break;
		case OTDR_GPIO4:
			set_otdr_pin(OTDR_PIN4, data);
			break;
		case OTDR_GPIO5:
			set_otdr_pin(OTDR_PIN5, data);
			break;
		default:
			printf("Invalid pin number\n");
			return -1;
	}

	return 0;
}

int get_otdr_gpio (unsigned int gpio, unsigned char *data)
{
	unsigned char otdr = 0;

	if ((gpio < 0) ||(gpio > 5)) {
		printf("Invalid pin number\n");
		return -1;
	}

	otdr = *((unsigned char *)(cpld_base + CONT_REG4));

	switch (gpio) {
		case OTDR_GPIO1:
			*data = (otdr & (0x1 << OTDR_PIN1)) ? 1:0;
			break;
		case OTDR_GPIO2:
			*data = (otdr & (0x1 << OTDR_PIN2)) ? 1:0;
			break;
		case OTDR_GPIO3:
			*data = (otdr & (0x1 << OTDR_PIN3)) ? 1:0;
			break;
		case OTDR_GPIO4:
			*data = (otdr & (0x1 << OTDR_PIN4)) ? 1:0;
			break;
		case OTDR_GPIO5:
			*data = (otdr & (0x1 << OTDR_PIN5)) ? 1:0;
			break;
		default:
			printf("Invalid pin number\n");
			return -1;
	}

	return 0;
}

