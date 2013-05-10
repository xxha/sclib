
#ifndef __ENV_H
#define __ENV_H

#include <stdint.h>

#define NEED_WRITE  1
#define BOOTARGS    "bootargs"
#define BOOTPART    "bootpart"

#define DEFAULT_ENV \
        "bootpart=0x80000:0x4fffff,0x6180000:0xeffffff\0" \
        "bootargs=console=ttyAMA1,115200n8 kgdboc=ttyAMA1 root=/dev/mtdblock3 rootfstype=romfs init=/linuxrc lpj=3112960 quiet\0" \
        "\0"

typedef	struct {
	uint32_t	crc;		/* CRC32 over data bytes */
	uint8_t		data[1];	/* Environment data */
} env_t;

env_t *initEnv(char *name);
int setDefault(uint8_t *env, char *def);
char *getEnv(uint8_t *env, char *name);
int setEnv(uint8_t *env, char *name, char *str);
int readEnv(env_t *env);
int writeEnv(env_t *env);

#endif
