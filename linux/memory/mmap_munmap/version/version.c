/* By Yanjun Luo, for MX27 gpio test with application software. */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>

/* Memory mapping definitions */
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define MMAP_BASE_ADDR	0x10028000
#define FCTL 0x10
#define PRODUCT_REVISION 0x20
#define SILICON_REVISION 0x24


volatile void *map_base = NULL; 
int fd = 0;

int ver_init()
{
	int i, rc, pin, dest_addr, id, retval, flag = 0;
	char *current, action, port;
	unsigned long readval, writeval;

	unsigned int temp = 0, temp1 = 0;;
	
	if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1){
		printf("Error opening /dev/mem\n");
		return -1;
	}

	/* Map one page */
	map_base = mmap((void *)0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_BASE_ADDR & ~MAP_MASK);
	if(map_base == (void *) -1){
		printf("Error mapping memory\n");
		close(fd);
		return -1;
	}
}

int ver_exit()
{
	/* Unmap previously mapped page */
	if(munmap((void *)map_base, MAP_SIZE) == -1){
		printf("Error unmapping memory\n");
		close(fd);
		return -1;
	}
	
 	close(fd);
}

int main()
{
	volatile unsigned int product_ver = 0; 
	volatile unsigned int silicon_ver = 0;
	volatile unsigned int fctl = 0;
	ver_init();

	fctl = *((unsigned long*)(map_base + FCTL));
	product_ver = *((unsigned long*)(map_base + PRODUCT_REVISION));
	silicon_ver = *((unsigned long*)(map_base + SILICON_REVISION));

	printf("fctl = 0x%x\n", fctl);
	printf("product_ver = 0x%x\n", product_ver);
	printf("silicon_ver = 0x%x\n", silicon_ver);

	ver_exit();
}

