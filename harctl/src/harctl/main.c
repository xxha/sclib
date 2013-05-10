#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harctl.h"

static void usage()
{
	printf("usage: harctl command {args} {parameters}\n"
			"where  command := { printenv | setenv | getenv \n"
			"       setdefault | getbootorder | setbootorder\n"
			"       getcmdline | setcmdline | getlog | writepart}\n"
			"       args := {bootargs | bootpart | watchdog}\n");
}

static int do_help(int argc, char **argv)
{
	usage();
}

int printenv(int argc, char **argv)
{
	char *ptr = harEnv_List();

	while (*ptr)
	{
		printf(" %s\n", ptr);
		ptr += strlen(ptr) + 1;
	}

	return 0;
}

int geten(int argc, char **argv)
{
	char *ptr;

	if (!*argv)
	{
		fprintf(stderr, "No args, please input args!\n");
		return 0;
	}

	ptr = harEnv_Get(*argv);

	if (ptr)
		printf(" %s = %s\n", *argv, ptr);

	return 0;
}

int seten(int argc, char **argv)
{
	int err;

	if (!*argv)
	{
		fprintf(stderr, "No args, please input args!\n");
		return 0;
	}

	err = harEnv_Set(*argv, *(argv + 1));
	harEnv_Save();
	return err;
}

int setdefault()
{
	int err = harEnv_setDefault();
	harEnv_Save();

	return err;
}

int getbootorder()
{
	char *param = harEnv_getBootOrder();
	printf("bootpart=%s\n", param);

	return 0;
}

int setbootorder(int argc, char **argv)
{
	int err = harEnv_setBootOrder(*argv);
	harEnv_Save();

	return err;
}

int getcmdline()
{
	char *param = harEnv_getCmdline();
	printf("bootargs=%s\n", param);

	return 0;
}

int setcmdline(int argc, char **argv)
{
	int err = harEnv_setCmdline(*argv);
	harEnv_Save();

	return err;
}

int getlog()
{
	char *log = harAPI_getLog();
	printf("x-loader logs :\n");

	while (*log)
	{
		printf("%s\n", log);
		log += strlen(log) + 1;
	}

	return 0;
}

int writepart(int argc, char **argv)
{
	int err;
	size_t size;
	char *buf;
	FILE *image, *stream;

	if (*argv == NULL)
	{
		fprintf(stderr, "No partition name, please input partition name!\n");
		return -1;
	}

	if (*(argv + 1) == NULL)
	{
		fprintf(stderr, "No file to write, please input filename.\n");
		return -1;
	}

	image = fopen(*(argv + 1), "rb");
	if (image == NULL)
	{
		fprintf(stderr, "ERROR: Image open failed!\n");
		return -1;
	}

	fseek(image, 0L, SEEK_END);
	size = (size_t) ftell(image);

	// How best to do this? Allocated size needs to be a multiple of
	// page sizes. Hard-coded right now to 2k.
	buf = malloc((size + 0x7ff) & ~0x7ff);

	if (buf == NULL)
	{
		fprintf(stderr, "ERROR: Buffer malloc failed!\n");
		return -1;
	}

	fseek(image, 0L, SEEK_SET);
	if (fread(buf, 1, size, image) != size)
	{
		fprintf(stderr, "ERROR: Image read failed!\n");
	}

	err = harAPI_writePartition(*argv, size, buf);
	if (err)
	{
		fprintf(stderr, "ERROR: Write partition failed!\n");
		return -1;
	}

	fclose(image);
	return 0;
}

static const struct cmd
{
	const char *cmd;
	int (*func)(int argc, char **argv);
} cmds[] = {
		{ "help", do_help },
		{ "printenv", printenv },
		{ "setenv", seten },
		{ "getenv", geten },
		{ "setdefault", setdefault },
		{ "getbootorder", getbootorder },
		{ "setbootorder", setbootorder },
		{ "getcmdline", getcmdline },
		{ "setcmdline", setcmdline },
		{ "getlog", getlog },
		{ "writepart", writepart },
		{ 0 }
};

int matches(const char *cmd, const char *pattern)
{
	int len = strlen(cmd);
	if (len != strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}

static int do_cmd(const char *argv0, int argc, char **argv)
{
	const struct cmd *c;
	int err;

	err = harAPI_Init();
	if (err)
		return err;

	for (c = cmds; c->cmd; ++c)
	{
		if (matches(argv0, c->cmd) == 0)
			return c->func(argc - 1, argv + 1);
	}

	fprintf(stderr, "Object \"%s\" is unknown, try \"harctl help\".\n", argv0);
	return -1;
}

int main(int argc, char **argv)
{
	if (argc <= 1)
		usage();
	else
		do_cmd(argv[1], argc - 1, argv + 1);

	return 0;
}
