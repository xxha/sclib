
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "harctl/config.h"
#include "harctl/env.h"
#include "harctl/nand.h"

static nand_t *nandEnv;

env_t *initEnv(char *name)
{
	env_t *buf;

	nandEnv = nandOpen(name, O_RDWR);

	if (nandEnv) {
		buf = calloc(nandEnv->size, 1);
		if (!buf)
			nandClose(nandEnv);
	} else
		buf = NULL;

	return buf;
}

int setDefault(uint8_t *env, char *def)
{
	memset(env, 0, sizeof(env));

	while (*def)
	{
		strcpy(env, def);
		env += strlen(def) + 1;
		def += strlen(def) + 1;
	}
	*env = '\0';
	return 0;
}

char *getEnv(uint8_t *env, char *name)
{
	while (*env)
	{
		if (!strncmp(env, name, strlen(name)))
		{
			env += strlen(name);
			if (*env == '=')
				return env + 1;
		}
		env += strlen(env) + 1;
	}
	return NULL;
}

int setEnv(uint8_t *env, char *name, char *str)
{
	char *rest;
	char *tmp, *backup;
	rest = calloc(128, 1024);
	backup = rest;
	while (*env)
	{
		/*locate the parameter in env*/
		if (!strncmp(env, name, strlen(name)))
		{
			tmp = env + strlen(env) + 1;
			/*backup the rest env*/
			while (*tmp)
			{
				strcpy(rest, tmp);
				rest += strlen(rest) + 1;
				tmp += strlen(tmp) + 1;
			}
			*rest = '\0';

			/*make sure parameter is correct*/
			if (*(env + strlen(name)) == '=')
			{
				/*if no args, delete parameter*/
				if (str == NULL)
				{
					rest = backup;
					while (*rest)
					{
						strcpy(env, rest);
						env += strlen(env) + 1;
						rest += strlen(rest) + 1;
					}
					*env = '\0';
					free(backup);
					return 0;

				}
				env += strlen(name) + 1;
			}
			else
			{
				env += strlen(env) + 1;
				continue;
			}

			/*change env only if args is different*/
			if (strcmp(env, str) == 0)
			{
				fprintf(stderr, "Args the same, ignore!\n");
				return 0;
			}
			else
			{
				strcpy(env, str);
				env += (strlen(str) + 1);
				rest = backup;
				while (*rest)
				{
					strcpy(env, rest);
					env += strlen(env) + 1;
					rest += strlen(rest) + 1;
				}
				*env = '\0';
				free(backup);
				return 0;
			}
		}
		env += strlen(env) + 1;
	}

	/*create new parameter and args*/
	if (*str == '\0')
	{
		fprintf(stderr, "No args. please input args\n");
		return 0;
	}
	strcpy(env, name);
	strcat(env, "=");
	strcat(env, str);
	env += strlen(str) + strlen(name) + 2;
	*env = '\0';

	free(backup);
	return 0;
}

int readEnv(env_t *env)
{
	uint8_t *buf = (uint8_t *) env;
	uint32_t page = 0;
	uint32_t endPage = nandEnv->size / nandEnv->page - 1;
	uint32_t crc;
	int size = 0, i = sizeof(env->crc);

	page = readNandPages(nandEnv, buf, page, 1, endPage);
	if (!page)
	{
		fprintf(stderr, "ERROR: Cannot read env page\n");
		return -1;
	}

	// env area is a bunch of \0 terminated strings where the end is an
	// empty string.
	do
	{
		while (buf[i++]) {
			if (i >= nandEnv->page)
			{
				i -= nandEnv->page;
				size += nandEnv->page;
				buf += nandEnv->page;
				page = readNandPages(nandEnv, buf, page, 1, endPage);
				if (!page)
				{
					fprintf(stderr, "ERROR: Cannot read env page\n");
					return -1;
				}
			}
		}

		if (i >= nandEnv->page)
		{
			i -= nandEnv->page;
			size += nandEnv->page;
			buf += nandEnv->page;
			page = readNandPages(nandEnv, buf, page, 1, endPage);
			if (!page)
			{
				fprintf(stderr, "ERROR: Cannot read env page\n");
				return -1;
			}
		}
	} while (buf[i]);

	size += i + 1 - sizeof(env->crc);
	crc = crc32(0, env->data, size);

	if (env->crc != crc)
	{
		fprintf(stderr, "ERROR: env CRC mismatch %08x != %08x\n", env->crc, crc);
		return 0;
	}

	return 0;
}

int writeEnv(env_t *env)
{
	uint8_t *buf = (uint8_t *) env;
	uint32_t endPage = nandEnv->size / nandEnv->page - 1;
	uint32_t page;
	uint32_t crc;
	int size, i;

	// env area is a bunch of \0 terminated strings where the end is an
	// empty string.
	i = sizeof(env->crc);
	do
	{
		while (buf[i++]);
	} while (buf[i++]);

	size = i - sizeof(env->crc);

	env->crc = crc32(0, env->data, size);

	page = (size + nandEnv->page - 1) / nandEnv->page;

	if (eraseNandPages(nandEnv, 0, endPage))
	{
		fprintf(stderr, "ERROR: Cannot erase env pages\n");
		return -1;
	}

	page = writeNandPages(nandEnv, buf, 0, page, endPage);
	if (!page)
	{
		fprintf(stderr, "ERROR: Cannot write env pages\n");
		return -1;
	}

	return 0;
}
