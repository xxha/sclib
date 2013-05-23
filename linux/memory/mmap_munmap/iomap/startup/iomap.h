/********************************************************************
	created:	2006/08/10
	created:	10:8:2006   16:43
	filename: 	f:\home\work\v100\include\iomap.h
	file path:	f:\home\work\v100\include
	file base:	iomap
	file ext:	h
	author:		Melody, Sword
	
	purpose:	IO Map routines.
*********************************************************************/

#ifndef _VEEXINC_IOMAP_H
#define _VEEXINC_IOMAP_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define IOMAP_MAJOR			42
#define IOMAP_MAX_DEVS		16

/* define to use readb and writeb to read/write data */
/* the device structure */
typedef struct Iomap 
{
	unsigned long base;
	unsigned long size;
	unsigned long busBitSize;
	char* ptr;
} Iomap;

#define IOMAP_GET	_IOR(0xbb, 0, Iomap)
#define IOMAP_SET	_IOW(0xbb, 1, Iomap)
#define IOMAP_CLEAR	_IOW(0xbb, 2, long)


//Init all memory segments.
int	InitAllPhyMem();

//convert a physical memory address to virtual.
void* GetVirtualMemory(void* pPhyAddr, unsigned int* pRetUnit);


//show current memory segments.
void ShowPhyMemList();



#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif // _VEEXINC_IOMAP_H

