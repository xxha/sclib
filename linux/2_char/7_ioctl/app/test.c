/*
 * 	1. cat /proc/devices to look for "ioctl_test"
 *	2. mknod /dev/ioctl_test c 250 0
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <string.h>


#define FILE_PATH "/dev/ioctl_test"

/*
 * Ioctl definitions
 */
#define TEST_IOC_MAGIC 'n'

#define ZERO    _IO(TEST_IOC_MAGIC, 0)
#define FIRST   _IO(TEST_IOC_MAGIC, 1)

#define SECOND  _IOR(TEST_IOC_MAGIC, 2, int)
#define THIRD   _IOR(TEST_IOC_MAGIC, 3, struct read_struct)

#define FOURTH   _IOW(TEST_IOC_MAGIC, 4, int)
#define FIFTH    _IOW(TEST_IOC_MAGIC, 5, unsigned char)

#define SIXTH    _IOWR(TEST_IOC_MAGIC, 6, int)
#define SEVENTH  _IOWR(TEST_IOC_MAGIC, 7, unsigned char)

#define EIGHTH   _IO(TEST_IOC_MAGIC, 8)
#define NINTH    _IO(TEST_IOC_MAGIC, 9)
#define TENTH    _IO(TEST_IOC_MAGIC, 10)

#define TEST_IOC_MAXNR  10

struct read_struct {
        char read_string;
        int read_n;
};


int main(void)
{
	int fd;
	int read_num = 0;
	int write_num = 20130217;
	//char *read_str = NULL ;
	char *write_str = "I love you!";

	printf("run test\n");

	struct read_struct read_struct2;

	read_struct2.read_string = 'd';
	read_struct2.read_n = 5555;
/*
	read_struct2 = malloc(sizeof(struct read_struct));
	if (!read_struct2) {
		printf("malloc read_struct2 failed\n");
	}
	memset(read_struct2, 0, sizeof(struct read_struct));

	printf("malloc read_struct2 succeed\n");

	read_struct2->read_string = malloc(8);
	if (!read_struct2->read_string) {
		printf("malloc read_struct2->read_string failed\n");
	}
	memset(read_struct2->read_string, 0, 8);
*/	
	printf("malloc read_struct2->read_string succeed\n");

	fd = open(FILE_PATH, O_RDWR);
	if (fd == -1) {
		printf("open file failed\n");
	}
	printf("fd = %d\n", fd);

	if(ioctl(fd, ZERO) == -1)
		printf("ioctl zero failed\n");
	

	if (ioctl(fd, FIRST) == -1)
		printf("ioctl first failed\n");

	if (ioctl(fd, SECOND, &read_num) == -1) {
		printf("ioctl second failed\n");
	} else {
		printf("ioctl second read_num = %d\n", read_num);
	}

	if (ioctl(fd, THIRD, &read_struct2) == -1) { 
		printf("ioctl third failed\n");
	} else {
		printf("ioctl third read_struct2->read_string = %c\n", read_struct2.read_string);
		printf("ioctl third read_struct2->read_n = %d\n", read_struct2.read_n);
	}

//	free(read_struct2->read_string);
//	free(read_struct2);
	close(fd);
	return 0;
}

