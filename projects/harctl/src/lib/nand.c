#define _LARGEFILE64_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>

#include <sys/ioctl.h>

#include "harctl/config.h"
#include "harctl/nand.h"

#define MTD_PATH "/sys/devices/virtual/mtd/"

static char *getSysInfo(int partition, char *sys)
{
	static char str[128];
	char filename[128];
	int fd;

	*str = 0;

	sprintf(filename, MTD_PATH "mtd%d/%s", partition, sys);
	fd = open(filename, O_RDONLY);

	if (fd >= 0)
	{
		int len = read(fd, str, sizeof(str));
		if (len)
			str[len - 1] = 0;
		close(fd);
	}
	return str;
}

nand_t *nandOpen(char *name, int mode)
{
	int part = 0;
	nand_t *nand = malloc(sizeof(nand_t));
	char *sys;
	char device[128];

	if (nand) {

		while ((sys = getSysInfo(part, "name")) && strcmp(name, sys))
			part++;

		if (!sys) {
			free(nand);
			return NULL;
		}

		sprintf(device, "/dev/mtd%d", part);
		nand->fd = open(device, mode);

		if (nand->fd < 0) {
			free(nand);
			return NULL;
		}

		nand->size  = strtol(getSysInfo(part, "size"), NULL, 0);
		nand->erase = strtol(getSysInfo(part, "erasesize"), NULL, 0);
		nand->page  = strtol(getSysInfo(part, "writesize"), NULL, 0);
	}

	return nand;
}

int nandClose(nand_t *nand)
{
	int fd = nand->fd;
	free(nand);
	return close(fd);
}

int eraseNandPages(nand_t *nand, uint32_t page, int endPage)
{
	struct erase_info_user64 ei64;
	int ret;

	ei64.start = page * nand->page;
	ei64.length = (endPage + 1 - page) * nand->page;

	ret = ioctl(nand->fd, MEMERASE64, &ei64);
	if (ret)
	{
		if (ret == -1)
			ret = errno;
		fprintf(stderr, "Error erasing block(s): %llx - %s\n",
				ei64.start, strerror(ret));
	}

	return ret;
}

int readNandPages(nand_t *nand, uint8_t *buf, uint32_t page, int pageCount, int endPage)
{
	__kernel_loff_t seek;
	int ret;

	while (pageCount && page <= endPage)
	{
		seek = page * nand->page;
		ret = ioctl(nand->fd, MEMGETBADBLOCK, &seek);
		if (ret < 0)
		{
			if (ret == -1)
				ret = errno;
			fprintf(stderr, "Error checking bad block: %llx - %s\n",
					(off64_t)seek, strerror(ret));
			return 0;
		}
		if (!ret)
		{
			lseek64(nand->fd, (off64_t)seek, SEEK_SET);
			ret = read(nand->fd, buf, nand->page);
			if (ret < 0)
			{
				if (ret == -1)
					ret = errno;
				fprintf(stderr, "Error reading block: %llx - %s\n",
						(off64_t)seek, strerror(ret));
				return 0;
			}
			pageCount--;
			buf += nand->page;
		}
		page++;
	}

	return page;
}

int writeNandPages(nand_t *nand, uint8_t *buf, uint32_t page, int pageCount, int endPage)
{
	__kernel_loff_t seek;
	int ret;

	while (pageCount && page <= endPage)
	{
		seek = page * nand->page;
		lseek64(nand->fd, (off64_t)seek, SEEK_SET);
		ret = write(nand->fd, buf, nand->page);
		if (ret < 0)
		{
			if (ret == -1)
				ret = errno;
			fprintf(stderr, "Error writing block: %llx - %s\n",
					(off64_t)seek, strerror(ret));
			return 0;
		}
		ret = ioctl(nand->fd, MEMGETBADBLOCK, &seek);
		if (ret < 0)
		{
			if (ret == -1)
				ret = errno;
			fprintf(stderr, "Error checking bad block: %llx - %s\n",
					(off64_t)seek, strerror(ret));
			return 0;
		}
		if (!ret)
		{
			pageCount--;
			buf += nand->page;
		}
		page++;
	}

	return page;
}
