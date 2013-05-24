#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define	CONTROL_OFFSET0				0x00
#define	CONTROL_OFFSET1				0x01
#define	CONTROL_OFFSET2				0x02
#define	CONTROL_OFFSET3				0x03

#define	CPLD_BASE							0xC8000000
#define	ID_OFFSET							0x08
#define	CPLD_VER							0x0D
#define	ID_LEN								0x04

#define	ETH_CPU_DIS						0x01

/* Memory mapping definitions */
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1) /*4k size aligned*/

int util_fd;

volatile void *map_base;

void cpld_version()
{
	printf("CPLD Version is:%d\n", *((char *)(map_base+CPLD_VER)));
	
}

int util_init()
{
	if((util_fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1){
		printf("Error opening /dev/mem\n");
		return -1;
	}

  /* Map one page */
	map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, util_fd, CPLD_BASE & ~MAP_MASK);
	if(map_base == (void *) -1){
		printf("Error mapping memory\n");
		close(util_fd);
		return -1;
	}
}

int util_exit()
{
	int temp;
	
	close(util_fd);
	temp = munmap((void *)map_base, MAP_SIZE);
	if(temp == -1){
		printf("error unmapping memory\n");
		return -1;
	}
}

int main(int argc, char *argv[])
{
	int i;
	
	util_init();

	cpld_version();
	
//	return *((char *)(map_base+CPLD_VER));	
//	*((char *)(map_base+CONTROL_OFFSET3)) |= ETH_CPU_DIS;	
	util_exit();
}



