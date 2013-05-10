
#ifndef _NAND_H
#define _NAND_H

#include <stdint.h>
#include <fcntl.h>

#include <mtd/mtd-abi.h>

typedef struct {
	int fd;
	int size;               //partition size
	int erase;              //erase size
	int page;               //page size
} nand_t;

nand_t *nandOpen(char *name, int mode);
int nandClose(nand_t *nand);
int eraseNandPages(nand_t *nand, uint32_t page, int endPage);
int readNandPages(nand_t *nand, uint8_t *buf, uint32_t page, int pageCount, int endPage);
int writeNandPages(nand_t *nand, uint8_t *buf, uint32_t page, int pageCount, int endPage);

#endif
