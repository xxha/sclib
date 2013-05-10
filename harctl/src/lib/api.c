#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>

#include "harctl/config.h"
#include "harctl/env.h"
#include "harctl/nand.h"
#include "harctl.h"

#define LOG_BUF_SIZE     2048
#define BOOTLOG          "/sys/kernel/stmicro/bootlog"

static env_t *buf;

int harAPI_Init(void)
{
	int err;

	buf = initEnv(ENV_PARTITION_NAME);
	if (!buf)
	{
		fprintf(stderr, "ERROR: loading environment - %s\n", strerror(errno));
		return -1;
	}

	err = readEnv(buf);
	if (err)
		fprintf(stderr, "ERROR: reading env - %d\n", err);

	return err;
}

char *harEnv_List()
{
	return buf->data;
}

char *harEnv_Get(char *name)
{
	return getEnv(buf->data, name);
}

int harEnv_Set(char *name, char *value)
{
	return setEnv(buf->data, name, value);
}

int harEnv_Save(void)
{
	int err = writeEnv(buf);
	if (err)
		fprintf(stderr, "ERROR: writing env - %d\n", err);

	return err;
}

char *harEnv_getBootOrder()
{
	return getEnv(buf->data, BOOTPART);
}

int harEnv_setBootOrder(char *order)
{
	return setEnv(buf->data, BOOTPART, order);
}

char *harEnv_getCmdline()
{
	return getEnv(buf->data, BOOTARGS);
}

int harEnv_setCmdline(char *cmdline)
{
	return setEnv(buf->data, BOOTARGS, cmdline);
}

int harEnv_setDefault()
{
	setDefault(buf->data, DEFAULT_ENV);
	return 0;
}

char *harAPI_getLog()
{
	static char logbuf[LOG_BUF_SIZE];
	FILE *log;

	log = fopen(BOOTLOG, "r");
	if(!log) {
		fprintf(stderr, "ERROR: Open bootlog failed\n");
		return NULL;
	}

	memset(logbuf, 0, sizeof(logbuf));

	if (fread(logbuf, LOG_BUF_SIZE, 1, log) != 1)
		fprintf(stderr, "ERROR: Read bootlog failed\n");

	fclose(log);
	return logbuf;
}

int harAPI_writePartition(char *name, size_t size, char *data)
{
	nand_t *nand;
	uint32_t page, writePages, endPage;
	int ret;

	nand = nandOpen(name, O_RDWR);
	if (!nand)
	{
		fprintf(stderr, "Error opening NAND partition: %s\n", strerror(errno));
		return -1;
	}

	if (size <= nand->size)
	{
		writePages = (size + nand->page - 1) / nand->page;
		endPage = nand->size / nand->page - 1;

		ret = eraseNandPages(nand, 0, endPage);
		if (!ret) {
			page = writeNandPages(nand, data, 0, writePages, endPage);
			if (!page) {
				fprintf(stderr, "ERROR: Cannot write nand pages\n");
				ret = -1;
			}
		} else
			fprintf(stderr, "ERROR: Cannot erase nand pages\n");
	} else {
		fprintf(stderr, "ERROR: Partition too small for image: %d < %d\n",
				size, nand->size);
		ret = -1;
	}

	nandClose(nand);
	return ret;
}
