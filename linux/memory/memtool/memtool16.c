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

#define	MAP_LEN	0x1000000

#define	MAX_CMD_LEN		100

int util_fd = -1;

volatile void *map_base = NULL;

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
	map_base = mmap(0, MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, util_fd, CPLD_BASE & ~MAP_MASK);
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
	int i, j, flag, len = DEFAULT_LEN;
	unsigned int memaddr = 0;
	volatile void * mappedaddr = NULL;
	unsigned char cmd[MAX_CMD_LEN];
	unsigned char cmd_temp[MAX_CMD_LEN];
	FILE * cmd_fd = NULL;
	unsigned char * cmd_data = NULL;
	unsigned char * cmd1_data = NULL;
	unsigned char * cmd2_data = NULL;
	unsigned int showoffset, showlen, writeoffset;

	unsigned short writedata = 0;

	if(argc < 2){
		printf("Usage: ./memtool16 addr\n");
		exit(1);
	}

	util_init();

//	cpld_version();

	memaddr = strtoul(argv[1], NULL, 16);
	
	printf("\nMemtool for 16 bit bus, you input address is: 0x%x\n", memaddr);
	
	mappedaddr = mmap(0, MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, util_fd, memaddr & ~MAP_MASK);
	if(mappedaddr == (void *) -1){
		perror("Error mapping memory");
		close(util_fd);
		return -1;
	}

	printf("    | ");

	for(i = 0; i < 16; i += 2){
		printf("  %02x ", i);
	}
	
	printf("\n-----------------------------------------------------");
	
	flag = 0;
	for(i = 1, j = 0; i <= len; i += 2){
		if(flag == 0){
			printf("\n[%02x]| ", j*0x10);
			j += 1;
			flag = 1;
		}
			
		printf("%04x ", *((unsigned short *)(mappedaddr+i)));
		if(i%4 == 0){
			 flag = 0;
		}
	}	
	printf("\n");

	cmd_fd = fopen("/dev/console", "r");
	if( cmd_fd == NULL){
		perror("Console open");
		exit(1);
	}

	for(;;){
		printf("\nPlease input a command, like: d 0x100 0x10\n:");
	
		if(NULL == fgets(cmd, MAX_CMD_LEN, cmd_fd)){
			printf("Please input a command.\n");
			continue;
		}

		memset(cmd_temp, 0, MAX_CMD_LEN);

		switch(cmd[0]){
			case 'd':           /* display start_addr size*/
			case 'D':
				cmd_data = strchr(cmd, ' ');
				if(cmd_data == NULL){
					printf("Please input a valid command.\n");
					continue;
				}
				cmd_data ++;
				cmd1_data = strchr(cmd_data, ' ');
				if(cmd1_data != NULL){
					memcpy(cmd_temp, cmd_data, cmd1_data-cmd_data);
				}else{
					printf("Please input valid command.\n");
					continue;
				}
				showoffset = strtoul(cmd_temp, NULL, 16);
				if(showoffset + showlen >= MAP_LEN){
				       	showoffset = 0;
					showlen = DEFAULT_LEN;
					printf("The offset should less than 0x%x\n", MAP_LEN);
				}
				showlen = strtoul(cmd1_data, NULL, 16);
				
				if(showlen == 0) showlen = DEFAULT_LEN;
				
				printf("\nAddr: 0x%08x, len: 0x%x\n", memaddr+showoffset, showlen);
				memshow(mappedaddr+showoffset, showlen);
			break;
			
			case 'e':
			case 'E': /* set new memory value */

				cmd_data = strchr(cmd, ' ');
				cmd_data ++;
				
				writeoffset = strtoul(cmd_data, NULL, 16);
				
				if(writeoffset >= MAP_LEN){
					writeoffset = 0;
					printf("The offset should less than 0x%x\n", MAP_LEN);
				}

				for(i = 0; ; i += 2){
					memset(cmd, 0, MAX_CMD_LEN);
					printf("0x%08x[%04x]:", memaddr+writeoffset+i, *(volatile unsigned short *)(mappedaddr+writeoffset+i));
					if(NULL == fgets(cmd, MAX_CMD_LEN, cmd_fd)){  /* input a value */
						printf("Please input a command.\n");
						continue;
					}
					
//					printf("cmd: %s, 0:%x\n", cmd, cmd[0]);
					
					if(cmd[0] == '\n'){
//						printf("Aborted\n");
						break;	
					}
					
					for(j = 0; ; j ++){
						if(cmd[j] == '\n') break;
						if(isxdigit(cmd[j]) == 0){
							printf("Please input a valid hex data.");
							break;
						}
					}
					
					if(cmd[j] != '\n') break;
					
					writedata = strtoul(cmd, NULL, 16);
//					printf("We write 0x%x to %p\n", writedata, memaddr+writeoffset+i);
					*(volatile unsigned short *)(mappedaddr+writeoffset+i) = writedata; /* write value */

				}
				
			break;
			
			case 'q':
			case 'Q': /* close and quit */

				i = munmap((void *)mappedaddr, MAP_LEN);
				if(i == -1){
					printf("error unmapping memory\n");
					return -1;
				}
			
				util_exit();
				
				fclose(cmd_fd);
				
				exit(0);
			break;
			
			default:
				printf("Please input a command, like: d 0x100 0x10\n");
			continue;
		}

	}

}



void memshow(volatile unsigned char * showaddr, int showlen)
{
	unsigned int i, j, flag;
	
	printf("\n    | ");

	for(i = 0; i < 16; i += 2){
		printf("  %02x ", i);
	}
	
	printf("\n-----------------------------------------------------");
	
	flag = 0;
	for(i = 1, j = 0; i <= showlen; i += 2){
		if(flag == 0){
			printf("\n[%02x]| ", j*0x10);
			j += 1;
			flag = 1;
		}
			
		printf("%04x ", *((volatile unsigned char *)showaddr+i));
		if(i%8 == 0){
			 flag = 0;
		}
	}	
	printf("\n");
	
}
