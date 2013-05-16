/********************************************************************
	created:	2006/08/03
	created:	3:8:2006   19:21
	filename: 	iomap.c
	file path:	
	file base:	iomap
	file ext:	c
	author:		sword
	
	purpose:	Io Map routines.
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/io.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

//#include "comndef.h"

#include "iomap.h"


typedef struct  
{
	unsigned long	base;	//base address.
	unsigned int	size;	//memory size.
	unsigned int	unit;	//8,16,32.
	const char		*pDesc;	//description.

	volatile void	*pVirtalAddr;	//virtual address after map.
}PHY_MEM_DESC;


//Physical memory defines.
static PHY_MEM_DESC	f_PhyMems[]=
{
	{0x10000000, 32*1024*1024,	8,	"Flash memory", NULL},
	{0x20000000, 8*1024,		8,	"CS2 Module registers 1", NULL},
	//{0x30000000, 256*1024,		16,	"CS3 Module registers 2", NULL},
	{0x40000000, 512*1024,		8,	"CPLD & board FPGA", NULL},
	{0x50000000, 4*1024*1024,	16,	"S1D13506 LCD controler", NULL},
	{0xFF000000, 16*1024,		16, "PPC IMMR", NULL},	
};


//size.
#define MAX_PHY_MEMS	(sizeof(f_PhyMems) / sizeof(PHY_MEM_DESC))


static int g_bMapped = 0;


//map physical memory.
static void* v100_mmap(const char* pDevName, unsigned long base, int size, int iunit)
{
	int		fd;
	Iomap	dev;
	int		i;
	void*   pRet = NULL;
	
	fd = open( pDevName, O_RDWR | O_SYNC );
	if( fd < 0 )
	{
		printf("[MMAP] open failed, device=%s, base=%08X, uint=%d, err=%d\n", pDevName, base, iunit, errno);
		return NULL;
	}	

	dev.base = base;
	dev.size = size;
	dev.busBitSize = iunit;

	if( ioctl(fd, IOMAP_SET, &dev) < 0 )
	{
		//printf("ioctl IOMAP_SET fail - error %d\n", errno);
		//return 1; //don't do anything in debug mode
	}

	pRet = (void* )mmap( 0, dev.size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 ); 
	if( (long)pRet == -1 )
        pRet = NULL;

	if( !pRet )
	{
		printf("[MMAP] failed, device=%s, base=%08X, uint=%d\n", pDevName, base, iunit);
	}
	else
	{
		//printf("[MMAP] OK, device=%s, base=%08X, uint=%d, addr=%08X\n", pDevName, base, iunit, pRet);
	}

	return pRet;
}


//map all physical memory.
int	InitAllPhyMem()
{
	int				nRet = 0;
    int             i;
    char			csDevName[128];
	PHY_MEM_DESC*	pPhyMems = f_PhyMems;
	int				bAllZero = 1;

	for( i = 0; i < MAX_PHY_MEMS; i++ )
	{
		//device name.
		sprintf(csDevName, "/dev/iomap%d", i);

		//map device.
		pPhyMems[i].pVirtalAddr = (volatile void* )v100_mmap(csDevName, pPhyMems[i].base, pPhyMems[i].size, pPhyMems[i].unit);

		if( NULL == pPhyMems[i].pVirtalAddr ) 
            nRet = 1;
		else
			bAllZero = 0;		
	}

	return nRet;
}

//convert physical memory to virtual memory.
void* GetVirtualMemory(void* pPhyAddr, unsigned int* pRetUnit)
{
	void*			pRet     = NULL;
	unsigned long	lPhyAddr = (unsigned long)pPhyAddr;

    int i;

	//Check status.
	if( !g_bMapped ) 
	{
		InitAllPhyMem();
		g_bMapped = 1;
	}

	for(i = 0; i < MAX_PHY_MEMS; i++)
	{
		//check range.
		if(lPhyAddr >= f_PhyMems[i].base && lPhyAddr <= f_PhyMems[i].base + f_PhyMems[i].size)
		{
			//bingo, in range.
			lPhyAddr -= f_PhyMems[i].base;
			pRet = (void*)((unsigned long)f_PhyMems[i].pVirtalAddr + lPhyAddr);
			if( pRetUnit ) 
            {
                *pRetUnit = f_PhyMems[i].unit;
            }

			break;
		}
	}

	return pRet;
}

void ShowPhyMemList()
{
    int i;
	printf("Memory list:\n");

	for( i = 0; i < MAX_PHY_MEMS; i++ )
	{
		printf("[%08X - %08X] %8dK, unit=%-3d, %s\n",
			f_PhyMems[i].base, f_PhyMems[i].base+f_PhyMems[i].size,
			f_PhyMems[i].size/1024, f_PhyMems[i].unit, f_PhyMems[i].pDesc);
	}

    return;
}
