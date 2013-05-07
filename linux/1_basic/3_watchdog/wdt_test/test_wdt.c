#include <stdio.h>   
#include <stdlib.h>
#include <string.h>   
#include <sys/types.h>   
#include <sys/stat.h>   
#include <unistd.h>   
#include <fcntl.h>   
#include <sys/ioctl.h>   
#include <errno.h>   
#include <sys/time.h>   
#include <unistd.h>   
#include <time.h>   
#include <getopt.h>   
#include <sys/signal.h>   
#include "watchdog.h"   
#define  OPTION_REBOOT		0
#define  OPTION_BASIC		1

#define  REBOOT		"reboot"

int zsleep(int millisecond)   
{   
	unsigned long usec;   
	usec=1000*millisecond;   
	usleep(usec);   
}
/*      
void keep_alive(void)
{
    int dummy;

    ioctl(fd, WDIOC_KEEPALIVE, &dummy);
}

void wdt_getstatus(int *arg)
{
    ioctl(fd, WDIOC_KEEPALIVE, *arg);
}

void keep_getbootstatus(int *arg)
{
    ioctl(fd, WDIOC_KEEPALIVE, *arg);
}
*/
int Init()   
{
	int fd;
	//open device file
	fd = open("/dev/watchdog",O_RDWR);
	if(fd < 0)
	{
	    printf("device open fail\n");
	    return -1;
	}
	else
	    printf("enable watchdog\n");

	return fd;
}

int main(int argc,char **argv)   
{
        int fd,ch;
        int i,j,k=1,option,timeout;
	char *arg;
        struct watchdog_info wi;
        fd=Init();
	if(argv[1]!=NULL){
		if(strcmp(argv[1],REBOOT)==0){
			printf("reboot mode\n");
			arg = argv[2];
			i = strtoul(arg,&arg,10);
			option = OPTION_REBOOT;
		}
		else{
			printf("invalid option arguments,only do the basic action\n");
			option = OPTION_BASIC;
		}	
	}
	else{
		printf("only basic action\n");
		option = OPTION_BASIC;
	}
	if(option == OPTION_BASIC){
        	//read watchdog information
        	ioctl(fd,WDIOC_GETSUPPORT,&wi); 
        	printf("%d,%s\n",wi.options,wi.identity);

        	//set watchdog timeout   
        	//set the timeout is 10s,if success return 0,else return -1   
        	i=5;
		j=ioctl(fd,WDIOC_SETTIMEOUT,&i);
		if(j==0)
            		printf("Set watchdog timeout success!\nSet watchdog timeout: %ds\n",i*2);
		else
	    		printf("Set watchdog timeout failed!\n");

        	//read watchdog timeout
		j=ioctl(fd,WDIOC_GETTIMEOUT,&i);
		if(j==0)
            		printf("Read watchdog timeout success!\nRead watchdog timeout: %ds\n",i*2);
		else
	    		printf("Read watchdog timeout failed!\n");

        	//disable watchdog
        	close(fd);
		printf("disable watchdog\n");
	}
	else{
 		//read watchdog information
                ioctl(fd,WDIOC_GETSUPPORT,&wi);
                printf("%d,%s\n",wi.options,wi.identity);

                //set watchdog timeout   
                //set the timeout is 10s,if success return 0,else return -1   
                timeout = i/2;
                j=ioctl(fd,WDIOC_SETTIMEOUT,&timeout);
                if(j==0)
                        printf("Set watchdog timeout success!\nSet watchdog timeout: %ds\n",timeout*2);
                else
                        printf("Set watchdog timeout failed!\n");

                //read watchdog timeout
                j=ioctl(fd,WDIOC_GETTIMEOUT,&timeout);
                if(j==0)
                        printf("Read watchdog timeout success!\nRead watchdog timeout: %ds\n",timeout*2);
                else
                        printf("Read watchdog timeout failed!\n");

                timeout = timeout*2;
                while(1){
                        printf("I 'll do the reboot after %d seconds\n",timeout--);
			sleep(1);
                }
                //disable watchdog
                close(fd);
                printf("disable watchdog\n");
	}
        return 0;
}

