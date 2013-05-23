/* Yanjun Luo, EEPROM operation for V300. */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include "libeeprom.h"

#include "i2c-dev.h"

#define	EEPROM_I2C_ADDR	0x51

#define	EEPROM_DEV				"/dev/i2c-0"


int fd = 0;


int eeprom_open()
{
	int r;

	fd = open(EEPROM_DEV, O_RDWR);
	if(fd <= 0)
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	// set working device
	if( ( r = ioctl(fd, I2C_SLAVE, EEPROM_I2C_ADDR)) < 0)
	{
		fprintf(stderr, "Error eeprom_address set: %s\n", strerror(errno));
		return -1;
	}
	
	return 0;
}

int eeprom_close()
{
	return close(fd);
}

unsigned char eeprom_read_byte(unsigned int addr)
{
		return i2c_smbus_read_byte_data(fd, addr);
}

int eeprom_write_byte(unsigned int addr, unsigned char data)
{
	int r;
	
	if(addr > MAXSIZE) return -1;
	
	if( ( r = i2c_smbus_write_byte_data(fd, addr, data)) < 0)
		{
			fprintf(stderr, "Error EEPROM write: %s\n", strerror(errno));
			return -1;
		}
		
		usleep(10000);
		
		return 0;
}

int eeprom_read_buf(unsigned int start, unsigned int len, unsigned char * buf)
{
	int i = 0;
	
	if((start + len) > MAXSIZE) return -1;
	
	for(i = 0; i < len; i ++){
		buf[i] = eeprom_read_byte(start+i);
	}

	return 0;	
}

int eeprom_write_buf(unsigned int start, unsigned int len, unsigned char * buf)
{
	int i = 0;
	int r = 0;
	
	if((start + len) > MAXSIZE) return -1;
	
	for(i = 0; i < len; i ++){
		r = eeprom_write_byte(start+i, buf[i]);
		if(r == -1) return r;
	}

	return 0;	
}

