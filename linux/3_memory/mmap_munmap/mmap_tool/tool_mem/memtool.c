#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define	CONTROL_OFFSET0				0x00
#define	CONTROL_OFFSET1				0x01
#define	CONTROL_OFFSET2				0x02
#define	CONTROL_OFFSET3				0x03

#define	CPLD_BASE				0xC8000000
#define	ID_OFFSET				0x08
#define	CPLD_VER				0x0D
#define	ID_LEN					0x04

#define	DEFAULT_LEN				64

/* Memory mapping definitions */
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define	MAP_LEN	(32*1024*1024)

#define	MAX_CMD_LEN		100
#define NORFLASH 0x10000000

int util_fd = -1;

volatile void *map_base = NULL;

// The first 256 Bytes in flash
typedef struct st_IGMFlashOption
{
        unsigned char ucReserv0[16];  //reserved for Engineering development
        unsigned char ucSerialNo[14]; //Same as the serial number, 14 bytes in ASCII (2 spare bytes)
        unsigned char ucReserv1[2];
        unsigned char ucBarCode[15];  //Same as the bar code on the CPU board. 15 bytes in ASCII characters(1 spare byte)
        unsigned char ucReserv2[1];
        unsigned char ucOptTable[4];  //32 Bits Option Table (32 options total) for chassis related options
        unsigned char ucReserv3[76];  //Reserved for future
        unsigned char ucIGMConfig[128]; // For igmconfig

} st_IGMFlashOption;


#define IGM_IPADDRESS_LENGTH    4

typedef struct st_IGMConfig
{
    /* The number for current IGM machine */
    int nIGMNumber;

    /* Server IP address */
    unsigned char ucArrServerIP[IGM_IPADDRESS_LENGTH];

    /* Local IP address */
    unsigned char ucArrLocalIP[IGM_IPADDRESS_LENGTH];

    /* Local subnet mask */
    unsigned char ucArrNetmask[IGM_IPADDRESS_LENGTH];

    /* Local gateway IP address */
    unsigned char ucArrGateway[IGM_IPADDRESS_LENGTH];

    /* Server IP port number */
    int nPortNumber;

} st_IGMConfig;

void memshow(volatile unsigned char * showaddr, int showlen);

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
	map_base = mmap(0, MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, util_fd, NORFLASH);
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
	temp = munmap((void *)map_base, MAP_LEN);
	if(temp == -1){
		printf("error unmapping memory\n");
		return -1;
	}
}

int main(int argc, char *argv[])
{
        char buf[32];
        char cmd1[64];
        char cmd2[64];

        st_IGMConfig IGMConfig;
        st_IGMFlashOption flashOpt;

        memset(buf, 0, 32);
        memset(cmd1, 0, 64);
        memset(cmd2, 0, 64);
        memset(&IGMConfig, 0, sizeof(st_IGMConfig));
        memset(&flashOpt, 0, sizeof(st_IGMFlashOption));

	util_init();

        memcpy( &flashOpt, map_base, sizeof(st_IGMFlashOption));

        volatile char* pucIGMConfig = flashOpt.ucIGMConfig;
        memcpy( &IGMConfig, pucIGMConfig, sizeof(st_IGMConfig) );

        printf( "%d.%d.%d.%d : ", IGMConfig.ucArrLocalIP[0], IGMConfig.ucArrLocalIP[1],
                IGMConfig.ucArrLocalIP[2], IGMConfig.ucArrLocalIP[3] );
        printf( "%d.%d.%d.%d :", IGMConfig.ucArrNetmask[0], IGMConfig.ucArrNetmask[1],
                IGMConfig.ucArrNetmask[2], IGMConfig.ucArrNetmask[3] );

        printf( "%d.%d.%d.%d\n", IGMConfig.ucArrGateway[0], IGMConfig.ucArrGateway[1],
                IGMConfig.ucArrGateway[2], IGMConfig.ucArrGateway[3] );

	util_exit();
	return 0;
}



void memshow(volatile unsigned char * showaddr, int showlen)
{
	unsigned int i, j, flag;
	
	printf("\n    | ");

	for(i = 0; i < 16; i ++){
		printf("%02x ", i);
	}
	
	printf("\n-----------------------------------------------------");
	
	flag = 0;
	for(i = 1, j = 0; i <= showlen; i ++){
		if(flag == 0){
			printf("\n[%02x]| ", j);
			j += 1;
			flag = 1;
		}
			
		printf("%02x ", *((volatile unsigned char *)showaddr+i-1));
		if(i%16 == 0){
			 flag = 0;
		}
	}	
	printf("\n");
	
}
