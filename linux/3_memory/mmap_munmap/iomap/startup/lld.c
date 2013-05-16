/* lld.c - Source Code for Spansion's Low Level Driver */

/* v5.1.1 */
/**************************************************************************
* Copyright ??2005 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, One AMD Place, P.O. Box 3453 Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming Spansion's 
* Flash memory components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion's Flash memories. Spansion is not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this source code "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the source code herein.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE, 
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED 
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY, 
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  SPANSION WILL 
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR 
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE, 
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, 
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Copyright notice must be included with 
* this software, whether used in part or whole, at all times.  
*/

/* 
modification history
--------------------
01,20sep02,jgp written.
02,09sep03,jch Added delays for Clarifications C2 and C3.  These delays are   
   enabled with WAIT_4us_FOR_DATA_POLLING_BITS_TO_BECOME_ACTIVE and         
   PAUSE_BETWEEN_ERASE_SUSPENDS.                                            
03,02oct03,jch Improve lld_StatusGet's interleave handling, renamed functions
   to the new standard, implemented E2 and E3.
*/

/* NOTICE
MirrorBit flash devices requires 4us from the time    
a programming command is issued before the data polling 
bits can be read.  Without the delay, it is likely
that you will read invalid status from the flash.
The invalid status may lead the software to believe
that programming finished early without problems or 
that programming failed.  If your system has more 
than 4us of delay inherently, you don't need any 
additional delay.  Otherwise, change the #undef to 
a #define
WAIT_4us_FOR_DATA_POLLING_BITS_TO_BECOME_ACTIVE
in lld.h.  Make sure your optimization does not          
remove the delay loop.  You must replace DELAY_1us    
with a value which makes sense for your system.       
It is possible to suspend the erase operation 
with such frequency that it is unable to complete 
the erase operation and eventually times-out.                                            
Change the #undef to #define PAUSE_BETWEEN_ERASE_SUSPENDS 
in lld.h if you are using erase suspend and the 
following is true.
Time between suspends is less that 10 milliseconds    
AND total number of suspends per erase can exceed 5000.         
Make sure that your optimization does not remove the  
delay loop.  You must replace DELAY_10ms with a value 
which make sense in your system.
For more information, visit our web site at www.spansion.com,
email us at software@spansion.com or go to www.ask.amd.com.
*/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
//#include <sys/mman.h>
//#include <sys/ioctl.h>

#include "iomap.h"
#include "lld.h"

/* data masks */
#define DQ0_MASK (0x01)
#define DQ1_MASK (0x02)
#define DQ2_MASK (0x04)
#define DQ3_MASK (0x08)
#define DQ4_MASK (0x10)
#define DQ5_MASK (0x20)
#define DQ6_MASK (0x40)
#define DQ7_MASK (0x80)


#define TARGET_DQ0_MASK        (DQ0_MASK*LLD_DEV_MULTIPLIER)
#define TARGET_WRITE_BUF_ABORT (DQ1_MASK*LLD_DEV_MULTIPLIER)
#define TOGGLE_BIT_2_MASK      (DQ2_MASK*LLD_DEV_MULTIPLIER)
#define TARGET_DQ3_MASK        (DQ3_MASK*LLD_DEV_MULTIPLIER)
#define TARGET_DQ4_MASK        (DQ4_MASK*LLD_DEV_MULTIPLIER)
#define EXCEED_TIME_LIMIT_MASK (DQ5_MASK*LLD_DEV_MULTIPLIER)
#define TOGGLE_BIT_1_MASK      (DQ6_MASK*LLD_DEV_MULTIPLIER)
#define TARGET_DQ7_MASK        (DQ7_MASK*LLD_DEV_MULTIPLIER)

/* Address/Command Info */
#define NOR_AUTOSELECT_CMD               ((FLASHDATA)0x90909090)
#define NOR_CFI_QUERY_CMD                ((FLASHDATA)0x98989898)
#define NOR_CHIP_ERASE_CMD               ((FLASHDATA)0x10101010)
#define NOR_ERASE_SETUP_CMD              ((FLASHDATA)0x80808080)
#define NOR_PROGRAM_CMD                  ((FLASHDATA)0xA0A0A0A0)
#define NOR_RESET_CMD                    ((FLASHDATA)0xF0F0F0F0)
#define NOR_RESUME_CMD                   ((FLASHDATA)0x30303030)
#define NOR_SECSI_SECTOR_ENTRY_CMD       ((FLASHDATA)0x88888888)
#define NOR_SECSI_SECTOR_EXIT_SETUP_CMD  ((FLASHDATA)0x90909090)
#define NOR_SECSI_SECTOR_EXIT_CMD        ((FLASHDATA)0x00000000)
#define NOR_SECTOR_ERASE_CMD             ((FLASHDATA)0x30303030)
#define NOR_SUSPEND_CMD                  ((FLASHDATA)0xB0B0B0B0)
#define NOR_UNLOCK_BYPASS_ENTRY_CMD      ((FLASHDATA)0x20202020)
#define NOR_UNLOCK_BYPASS_PROGRAM_CMD    ((FLASHDATA)0xA0A0A0A0)
#define NOR_UNLOCK_BYPASS_RESET_CMD1     ((FLASHDATA)0x90909090)
#define NOR_UNLOCK_BYPASS_RESET_CMD2     ((FLASHDATA)0x00000000)
#define NOR_UNLOCK_DATA1                 ((FLASHDATA)0xAAAAAAAA)
#define NOR_UNLOCK_DATA2                 ((FLASHDATA)0x55555555)
#define NOR_WRITE_BUFFER_ABORT_RESET_CMD ((FLASHDATA)0xF0F0F0F0)
#define NOR_WRITE_BUFFER_LOAD_CMD        ((FLASHDATA)0x25252525)
#define NOR_WRITE_BUFFER_PGM_CONFIRM_CMD ((FLASHDATA)0x29292929)
static unsigned char WriteBufferProgramming = 0;

/* private functions */

/* Public Functions  */
/******************************************************************************
* 
* lld_ResetCmd - Writes a Software Reset command to the flash device
*
*
* RETURNS: void
*
* ERRNO: 
*/
void lld_ResetCmd
(
FLASHDATA * base_addr   /* device base address in system */
)
{       
    /* Write Software RESET command */
    FLASH_WR(base_addr, 0, NOR_RESET_CMD);
}

/******************************************************************************
* 
* lld_SectorEraseCmd - Writes a Sector Erase Command to Flash Device
*
* This function only issues the Sector Erase Command sequence.
* Data bar polling is not implemented in this function.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_SECTOR_ERASE_CMD
void lld_SectorEraseCmd
(
FLASHDATA * base_addr,                       /* device base address in system */
ADDRESS offset                           /* address offset from base address */
)
{       
  /* Issue Sector Erase Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_ERASE_SETUP_CMD);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  /* Write Sector Erase Command to Offset */
  FLASH_WR(base_addr, offset, NOR_SECTOR_ERASE_CMD);
}
#endif
/******************************************************************************
* 
* lld_ChipEraseCmd - Writes a Chip Erase Command to Flash Device
*
* This function only issues the Chip Erase Command sequence.
* Data bar polling is not implemented in this function.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_CHIP_ERASE_CMD
void lld_ChipEraseCmd
(
FLASHDATA * base_addr    /* device base address in system */
)
{       
  /* Issue Chip Erase Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_ERASE_SETUP_CMD);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  /* Write Chip Erase Command to Base Address */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_CHIP_ERASE_CMD);
}
#endif
/******************************************************************************
* 
* lld_ProgramCmd - Writes a Program Command to Flash Device
*
* This function only issues the Program Command sequence.
* Data bar polling is not implemented in this function.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_PROGRAM_CMD
void lld_ProgramCmd
(
FLASHDATA * base_addr,               /* device base address in system */
ADDRESS offset,                  /* address offset from base address */
FLASHDATA *pgm_data_ptr          /* variable containing data to program */
)
{       
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  /* Write Program Command */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_PROGRAM_CMD);
  /* Write Data */
  FLASH_WR(base_addr, offset, *pgm_data_ptr);
}
#endif
/******************************************************************************
* 
* lld_UnlockBypassEntryCmd - Writes Unlock Bypass Enter command to flash 
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_UNLOCK_BYPASS_ENTRY_CMD
void lld_UnlockBypassEntryCmd
(
FLASHDATA * base_addr                /* device base address in system */
)
{       
  /* Issue Unlock Bypass Enter Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_BYPASS_ENTRY_CMD);
}
#endif
/******************************************************************************
* 
* lld_UnlockBypassProgramCmd - Writes Unlock Bypass Pgm Sequence to Flash
*
* This function issues the Unlock Bypass Programming Sequence to device.
* Device must be in Unlock Bypass mode before using this function.
* Data bar polling is not implemented in this function.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_UNLOCK_BYPASS_PROGRAM_CMD
void lld_UnlockBypassProgramCmd
(
FLASHDATA * base_addr,               /* device base address in system */
ADDRESS offset,                  /* address offset from base address */
FLASHDATA *pgm_data_ptr          /* variable containing data to program */
)
{       
  /* Issue Unlock Bypass Program Command Sequence */
  /* Write Program Command */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_PROGRAM_CMD);
  /* Write Data */
  FLASH_WR(base_addr, offset, *pgm_data_ptr);
}
#endif
/******************************************************************************
* 
* lld_UnlockBypassResetCmd - Writes Unlock Bypass Exit command to flash 
*
* Issues Unlock Bypass Exit command sequence to Flash Device.  
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_UNLOCK_BYPASS_RESET_CMD
void lld_UnlockBypassResetCmd
(
FLASHDATA * base_addr               /* device base address in system */
)
{       
  /* Issue Unlock Bypass Exit Command Sequence */
  /* First Unlock Bypass Reset Command */
  FLASH_WR(base_addr, 0, NOR_UNLOCK_BYPASS_RESET_CMD1);
  /* Second Unlock Bypass Reset Command */
  FLASH_WR(base_addr, 0, NOR_UNLOCK_BYPASS_RESET_CMD2);
}
#endif
/******************************************************************************
* 
* lld_AutoselectEntryCmd - Writes Autoselect Command Sequence to Flash
*
* This function issues the Autoselect Command Sequence to device.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_AUTOSELECT_ENTRY_CMD
void lld_AutoselectEntryCmd
(
FLASHDATA * base_addr                                    /* device base address in system */
)
{       
  /* Issue Autoselect Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_AUTOSELECT_CMD);
}
#endif
/******************************************************************************
* 
* lld_AutoselectExitCmd - Writes Autoselect Exit Command Sequence to Flash
*
* This function resets the device out of Autoselect mode.
* This is a "wrapper function" to provide "Enter/Exit" symmetry in
* higher software layers.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_AUTOSELECT_EXIT_CMD
void lld_AutoselectExitCmd
(
FLASHDATA * base_addr   /* device base address in system */
)
{       
    lld_ResetCmd(base_addr);
}
#endif
/******************************************************************************
* 
* lld_SecSiSectorEntryCmd - Writes SecSi Sector Entry Command Sequence to Flash
*
* This function issues the Secsi Sector Entry Command Sequence to device.
* Use this function to Enable the SecSi Sector.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_SECSI_SECTOR_ENTRY_CMD
void lld_SecSiSectorEntryCmd
(
FLASHDATA * base_addr    /* device base address in system */
)
{       
  /* Issue SecSi Sector Entry Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_SECSI_SECTOR_ENTRY_CMD);
}
#endif
/******************************************************************************
* 
* lld_SecSiSectorExitCmd - Writes SecSi Sector Exit Command Sequence to Flash
*
* This function issues the Secsi Sector Exit Command Sequence to device.
* Use this function to Exit the SecSi Sector.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_SECTI_SECTOR_EXIT_CMD
void lld_SecSiSectorExitCmd
(
FLASHDATA * base_addr               /* device base address in system */
)
{       
  /* Issue SecSi Sector Exit Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  /* First Secsi Sector Reset Command */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_SECSI_SECTOR_EXIT_SETUP_CMD);
  /* Second Secsi Sector Reset Command */
  FLASH_WR(base_addr, 0, NOR_SECSI_SECTOR_EXIT_CMD);
}
#endif
/******************************************************************************
* 
* lld_WriteToBufferCmd - Writes "Write to Buffer Pgm" Command sequence to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
void lld_WriteToBufferCmd
(
FLASHDATA * base_addr,               /* device base address in system */
ADDRESS offset                   /* address offset from base address */
)
{       
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);  
  /* Write Write Buffer Load Command */
  FLASH_WR(base_addr, offset, NOR_WRITE_BUFFER_LOAD_CMD);
}
#endif
/******************************************************************************
* 
* lld_ProgramBufferToFlashCmd - Writes "Pgm Buffer To Flash" Cmd sequence to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
void lld_ProgramBufferToFlashCmd
(
FLASHDATA * base_addr,               /* device base address in system */
ADDRESS offset                   /* address offset from base address */
)
{       
  /* Transfer Buffer to Flash Command */
  FLASH_WR(base_addr, offset, NOR_WRITE_BUFFER_PGM_CONFIRM_CMD);
}
#endif
/******************************************************************************
* 
* lld_WriteBufferAbortResetCmd - Writes "Write To Buffer Abort" Reset to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
void lld_WriteBufferAbortResetCmd
(
FLASHDATA * base_addr        /* device base address in system */
)
{       
  /* Issue Write Buffer Abort Reset Command Sequence */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  /* Write to Buffer Abort Reset Command */
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_WRITE_BUFFER_ABORT_RESET_CMD);
}
#endif
/******************************************************************************
* 
* lld_ProgramSuspendCmd - Writes Suspend Command to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_PROGRAM_SUSPEND_CMD
void lld_ProgramSuspendCmd
(
FLASHDATA * base_addr,               /* device base address in system */
ADDRESS offset                   /* address offset from base address */
)
{       
  /* Write Suspend Command */
  FLASH_WR(base_addr, offset, NOR_SUSPEND_CMD);
}
#endif
/******************************************************************************
* 
* lld_EraseSuspendCmd - Writes Suspend Command to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_ERASE_SUSPEND_CMD
void lld_EraseSuspendCmd
(
FLASHDATA * base_addr,       /* device base address in system */
ADDRESS offset           /* address offset from base address */
)
{       
#ifdef PAUSE_BETWEEN_ERASE_SUSPENDS
  DELAY_MS(10);
#endif

  /* Write Suspend Command */
  FLASH_WR(base_addr, offset, NOR_SUSPEND_CMD);
}
#endif
/******************************************************************************
* 
* lld_EraseResumeCmd - Writes Resume Command to Flash
*
*
* RETURNS: DEVSTATUS
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_ERASE_RESUME_CMD
DEVSTATUS lld_EraseResumeCmd
(
FLASHDATA * base_addr,       /* device base address in system */
ADDRESS offset           /* address offset from base address */
)
{       
  DEVSTATUS dev_status = DEV_NOT_BUSY;

  /* Write Resume Command */
  FLASH_WR(base_addr, offset, NOR_RESUME_CMD);
  return(dev_status);
}
#endif
/******************************************************************************
* 
* lld_ProgramResumeCmd - Writes Resume Command to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_PROGRAM_RESUME_CMD
void lld_ProgramResumeCmd
(
FLASHDATA * base_addr,       /* device base address in system */
ADDRESS offset           /* address offset from base address */
)
{       
  /* Write Resume Command */
  FLASH_WR(base_addr, offset, NOR_RESUME_CMD);
}
#endif
/******************************************************************************
* 
* lld_CfiEntryCmd - Writes CFI Entry Command Sequence to Flash
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_CFI_ENTRY_CMD
void lld_CfiEntryCmd
(
FLASHDATA * base_addr /* device base address in system */
)
{       
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_CFI_QUERY_CMD);
}
#endif
/******************************************************************************
* 
* lld_CfiExitCmd - Writes Cfi Exit Command Sequence to Flash
*
* This function resets the device out of CFI Query mode.
* This is a "wrapper function" to provide "Enter/Exit" symmetry in
* higher software layers.
*
*
* RETURNS: void
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_CFI_EXIT_CMD
void lld_CfiExitCmd
(
FLASHDATA * base_addr   /* device base address in system */
)
{       
  lld_ResetCmd(base_addr);
}
#endif
/******************************************************************************
* 
* lld_Poll - Polls flash device for embedded operation completion
*
* Function polls the Flash device to determine when an embedded
*  operation has finished.  
*  Function performs a verify on the polling address if the device
*  returns from status routine in a non-busy/non-error state.
*
* <polling_type> variable is used to determine type of polling to perform.
*  This variable tells the status routine what mode the device is in.
*  Future versions of this function will also use the <polling_type>
*  variable to properly initialize and implement watchdog timers.
*  Acceptable values for <polling_type> are:
*   LLD_P_POLL_PGM 
*   LLD_P_POLL_WRT_BUF_PGM
*   LLD_P_POLL_SEC_ERS   
*   LLD_P_POLL_CHIP_ERS 
*
*
* RETURNS: DEVSTATUS
*
* ERRNO: 
*
*/
#ifndef REMOVE_LLD_POLL
DEVSTATUS lld_Poll
(
FLASHDATA * base_addr,          /* device base address in system */
ADDRESS offset,             /* address offset from base address */
FLASHDATA *exp_data_ptr,    /* expect data */
FLASHDATA *act_data_ptr,    /* actual data */
POLLING_TYPE polling_type   /* type of polling to perform */
)
{       
  	DEVSTATUS       dev_status;
  	unsigned int    polling_counter = 0xFFFFFFFF;
	
	#ifdef WAIT_4us_FOR_DATA_POLLING_BITS_TO_BECOME_ACTIVE
  	DELAY_US(4);
	#endif
  	/* set the WriteBuffer flag if writebuffer operation */
  	if (polling_type == LLD_P_POLL_WRT_BUF_PGM)
  	{
  		WriteBufferProgramming = 1;
  	}     
  	else
  	{
		WriteBufferProgramming = 0;
  	}           
	
  	/* Perform Polling Operation */
  	do
  	{
    	polling_counter--;
    	dev_status = lld_StatusGet(base_addr, offset);    	
  	}
  	while((dev_status == DEV_BUSY) && polling_counter);
	
  	/* read the actual data */
  	*act_data_ptr = FLASH_RD(base_addr, offset);
    
  /* 
	* if device returns status other than "not busy" then we
	*  have a polling error state. 
	*  Note: assumes the "while dev_busy" test above does not change!
	*
	* if device was "not busy" then verify polling location.
	*/
  	if(dev_status != DEV_NOT_BUSY)
  	{
		printf("Write error... \n");	    
	    if(dev_status == DEV_WRITE_BUFFER_ABORT)
	    {
	    	//printf("Write error, abort reset!\n");
			#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
			lld_WriteBufferAbortResetCmd(base_addr);      	
			#endif
	    }
	    else 
	    {
	    	//printf("Write error, software reset!\n");
	      	
	      	/* Issue software reset. */
	      	lld_ResetCmd(base_addr);
	    }
	    /* indicate to caller that there was an error */
		return(dev_status);
	}
	else 
	{
	  	//printf("\nNot busy! *exp_data_ptr = 0x%x, *act_data_ptr = 0x%x\n", *exp_data_ptr, *act_data_ptr);
	    /* Check that polling location verifies correctly */
	    if((*exp_data_ptr & LLD_DB_READ_MASK) == (*act_data_ptr & LLD_DB_READ_MASK))
	    {
	    	//printf("Write OK!\n");
	      	return(DEV_NOT_BUSY);
	    }
	    else 
		{
			printf("Write verify error!\n");
		  	return(DEV_VERIFY_ERROR);
		}
  	}
}
#endif

/******************************************************************************
*    
* lld_StatusGet - Determines Flash Status
*
*
* RETURNS: DEVSTATUS
*
*/
#define DQ6_TGL_DQ1_MASK (dq6_toggles >> 5)
#define DQ6_TGL_DQ3_MASK (dq6_toggles >> 3)
#define DQ6_TGL_DQ5_MASK (dq6_toggles >> 1)

#ifndef REMOVE_LLD_STATUS_GET
DEVSTATUS lld_StatusGet
(
FLASHDATA *  base_addr,      /* device base address in system */
ADDRESS      offset          /* address offset from base address */
)
{
  FLASHDATA dq1_mask          = (DQ1_MASK*LLD_DEV_MULTIPLIER);
  FLASHDATA dq2_mask          = (DQ2_MASK*LLD_DEV_MULTIPLIER);
  FLASHDATA dq3_mask          = (DQ3_MASK*LLD_DEV_MULTIPLIER);
  FLASHDATA dq5_mask          = (DQ5_MASK*LLD_DEV_MULTIPLIER);
  FLASHDATA dq6_mask          = (DQ6_MASK*LLD_DEV_MULTIPLIER);
  FLASHDATA dq6_toggles;
  volatile FLASHDATA status_read_1;
  volatile FLASHDATA status_read_2;

  status_read_1 = FLASH_RD(base_addr, offset);
  status_read_2 = FLASH_RD(base_addr, offset);

  /* DQ6 toggles ? */
  dq6_toggles = (status_read_1 ^ status_read_2) & dq6_mask;
    
  if (dq6_toggles)
  {
  	/* at least one device's DQ6 toggles */

    /* Checking WriteBuffer Abort condition: only check on the device that has DQ6 toggling */
    /* check only when doing writebuffer operation */
    if (WriteBufferProgramming && (DQ6_TGL_DQ1_MASK & status_read_2))
    {    
       /* read again to make sure WriteBuffer error is correct */
       status_read_1 = FLASH_RD(base_addr, offset);
       status_read_2 = FLASH_RD(base_addr, offset);
       dq6_toggles = (status_read_1 ^ status_read_2) & dq6_mask;
       // Don't return WBA if other device DQ6 and DQ1 
       // are not the same. They may still be busy.
       if ((dq6_toggles && (DQ6_TGL_DQ1_MASK & status_read_2)) &&
           !(DQ6_TGL_DQ1_MASK ^ (status_read_2 & dq1_mask))) 
          return DEV_WRITE_BUFFER_ABORT;
       else
          return DEV_BUSY;
    }

    /* Checking Timeout condition: only check on the device that has DQ6 toggling */
    if (DQ6_TGL_DQ5_MASK & status_read_2)
    {
       /* read again to make sure Timeout Error is correct */
       status_read_1 = FLASH_RD(base_addr, offset);
       status_read_2 = FLASH_RD(base_addr, offset);
       dq6_toggles = (status_read_1 ^ status_read_2) & dq6_mask;
       // Don't return TimeOut if other device DQ6 and DQ5 
       // are not the same. They may still be busy.
       if ((dq6_toggles && (DQ6_TGL_DQ5_MASK & status_read_2)) &&
           !(DQ6_TGL_DQ5_MASK ^ (status_read_2 & dq5_mask))) 
          return DEV_EXCEEDED_TIME_LIMITS;
       else
          return DEV_BUSY;
    }

    /* No timeout, no WB error */
    return DEV_BUSY;
  }
  else   /* no DQ6 toggles on all devices */
  {  	
    /* Checking Erase Suspend condition */
    status_read_1 = FLASH_RD(base_addr, offset);
    status_read_2 = FLASH_RD(base_addr, offset);
    if (((status_read_1 ^ status_read_2) & dq2_mask) == dq2_mask)   // All devices DQ2 toggling
    {	
		return DEV_SUSPEND;
    }       
    else if (((status_read_1 ^ status_read_2) & dq2_mask) == 0)   // All devices DQ2 not toggling
    {
    	//printf("step 5....\n");
    	return DEV_NOT_BUSY;
    }       
    else
    {    	
    	return DEV_BUSY;
    }       
  }
}
#endif
/******************************************************************************
* 
* lld_WriteBufferProgramOp - Performs a Write Buffer Programming Operation.
*
* Function programs a write-buffer overlay of addresses to data 
* passed via <data_buf>.
* Function issues all required commands and polls for completion.
*
* There are 4 main sections to the function:
*  Set-up and write command sequence
*  Determine number of locations to program and load buffer
*  Start operation with "Program Buffer to Flash" command
*  Poll for completion
*
* REQUIREMENTS:
*  Data in the buffer MUST be properly aligned with the Flash bus width.
*  No parameter checking is performed. 
*  The <word_count> variable must be properly initialized.  
*  Valid <byte_cnt> values: 
*   min = 1 byte (only valid when used with a single x8 Flash)
*   max = write buffer size in bytes * number of devices in parallel
      (e.g. 32-byte buffer per device, 2 x16 devices = 64 bytes)
*
* RETURNS: DEVSTATUS
*/
#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
DEVSTATUS lld_WriteBufferProgramOp
(
FLASHDATA *   base_addr,  /* device base address is system     */
ADDRESS   offset,     /* address offset from base address  */
WORDCOUNT word_count, /* number of words to program        */
FLASHDATA *data_buf   /* buffer containing data to program */
)
{       
  DEVSTATUS status;
  FLASHDATA write_data = 0;
  FLASHDATA read_data = 0;
  ADDRESS   last_loaded_addr;
  ADDRESS   current_offset;
  ADDRESS   end_offset;
  FLASHDATA wcount;

  /* Initialize variables */
  current_offset   = offset;
  end_offset       = offset + word_count - 1;
  last_loaded_addr = offset;
  
  //printf("Into write OP, word_count = %d\n", word_count);

  /* don't try with a count of zero */
  if (!word_count) 
  {
  	return(DEV_NOT_BUSY);
  }

  /* Issue Load Write Buffer Command Sequence */
  lld_WriteToBufferCmd(base_addr, offset);    
  
  //Roman add for test
  //DELAY_US(40);

  /* Write # of locations to program */
  wcount = (FLASHDATA)word_count - 1;
  wcount *= LLD_DEV_MULTIPLIER;
  FLASH_WR(base_addr, offset, wcount);

  /* Load Data into Buffer */
  while(current_offset <= end_offset)
  {
    /* Store last loaded address & data value (for polling) */
    last_loaded_addr = current_offset;
    write_data = (*data_buf);
    
    //printf("Write: 0x%x\n", write_data);
    
    //printf("Write data:[0x%x], curoff = %d, endoff = %d, lastoff = %d\n", (FLASHDATA)*data_buf,current_offset,end_offset,last_loaded_addr );

    /* Write Data */
    //FLASH_WR(base_addr, current_offset++, *data_buf++);
    FLASH_WR(base_addr, current_offset, *data_buf);    
    
    current_offset++;
    data_buf++;
  }

  /* Issue Program Buffer to Flash command */
  lld_ProgramBufferToFlashCmd(base_addr, last_loaded_addr);

  status = lld_Poll(base_addr, last_loaded_addr, &write_data, 
                    &read_data, LLD_P_POLL_WRT_BUF_PGM);
  return(status);
}
#endif

/******************************************************************************
* 
* lld_ProgramOp - Performs a standard Programming Operation.
*
* Function programs a single location to the specified data.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*/
#ifndef REMOVE_LLD_PROGRAM_OP
DEVSTATUS lld_ProgramOp
(
FLASHDATA * base_addr,      /* device base address is system */
ADDRESS offset,         /* address offset from base address */
FLASHDATA write_data    /* variable containing data to program */
)
{       
  FLASHDATA read_data = 0;

  lld_ProgramCmd(base_addr, offset, &write_data);
  return( lld_Poll(base_addr, offset, &write_data, &read_data, LLD_P_POLL_PGM) );
}
#endif
/******************************************************************************
* 
* lld_SectorEraseOp - Performs a Sector Erase Operation
*
* Function erases the sector containing <base_addr> + <offset>.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*
* ERRNO: 
*/
#ifndef REMOVE_LLD_SECTOR_ERASE_OP
DEVSTATUS lld_SectorEraseOp
(
FLASHDATA * base_addr,    /* device base address is system */
ADDRESS offset        /* address offset from base address */
)
{       
  FLASHDATA         expect_data = (FLASHDATA)0xFFFFFFFF;
  FLASHDATA         actual_data = 0;

  lld_SectorEraseCmd(base_addr, offset);
  return( lld_Poll(base_addr, offset, &expect_data, &actual_data, LLD_P_POLL_SEC_ERS) );
}
#endif
/******************************************************************************
* 
* lld_ChipEraseOp - Performs a Chip Erase Operation
*
* Function erases entire device located at <base_addr>.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*/
#ifndef REMOVE_LLD_CHIP_ERASE_OP
DEVSTATUS lld_ChipEraseOp
(
FLASHDATA * base_addr   /* device base address in system */
)
{       
  DEVSTATUS status;
  FLASHDATA expect_data = (FLASHDATA)0xFFFFFFFF;
  FLASHDATA actual_data = 0;

  lld_ChipEraseCmd(base_addr);

  status = lld_Poll(base_addr, 0, &expect_data, &actual_data, LLD_P_POLL_CHIP_ERS);

  /* if an error during polling, write RESET command to device */
  if(status != DEV_NOT_BUSY) lld_ResetCmd(base_addr);

  return(status);
}
#endif
/******************************************************************************
* 
* DelayMilliseconds - Performs a delay.  If you have a better way,
*                     edit the macro DELAY_MS in lld_target_specific.h
*
* RETURNS: void
*
*/
#ifndef REMOVE_DELAY_MILLISECONDS
void DelayMilliseconds(int milliseconds)
{
  int i;

  for (i = 0; i < milliseconds; i++)
    DELAY_US(1000);
}
#endif
/******************************************************************************
* 
* DelayMicroseconds - Performs a delay.  If you have a better way,
*                     edit the macro DELAY_US in lld_target_specific.h
*
* RETURNS: void
*
*/
#ifndef REMOVE_DELAY_MICROSECONDS
void DelayMicroseconds(int microseconds)
{
  int volatile i, j;

  for (j = 0; j < microseconds; j++)
    for(i = 0; i < DELAY_1us; i++) i = i;
}
#endif
/******************************************************************************
* 
* lld_ReadOp 
*
* RETURNS: data read
*
*/
#ifndef REMOVE_LLD_READ_OP
FLASHDATA lld_ReadOp
(
FLASHDATA * base_addr,    /* device base address is system */
ADDRESS offset        /* address offset from base address */
)
{
  return( FLASH_RD(base_addr, offset) );
}
#endif
/******************************************************************************
* 
* lld_ReadOp 
*
* RETURNS: three byte ID in a single int
*
*/
#ifndef REMOVE_LLD_GET_DEVICE_ID
unsigned int lld_GetDeviceId
(
FLASHDATA * base_addr   /* device base address in system */
)
{
  unsigned int id;

  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_AUTOSELECT_CMD);
  id  = (unsigned int)(FLASH_RD(base_addr, 0x0001) & 0x000000FF) << 16;
  id |= (unsigned int)(FLASH_RD(base_addr, 0x000E) & 0x000000FF) <<  8;
  id |= (unsigned int)(FLASH_RD(base_addr, 0x000F) & 0x000000FF)      ;
  lld_ResetCmd(base_addr);
  return(id);
}
#endif
/******************************************************************************
* 
* lld_memcpy   This function attempts to mimic the standard memcpy
*              function for flash.  It segments the source data
*              into page size chunks for use by Write Buffer Programming.
*
* RETURNS: DEVSTATUS
*
*/
#ifndef REMOVE_WRITE_BUFFER_PROGRAMMING
DEVSTATUS lld_memcpy
(
FLASHDATA * base_addr,    /* device base address is system */
ADDRESS offset,       /* address offset from base address */
WORDCOUNT word_cnt,   /* number of word to program */
FLASHDATA *data_buf        /* buffer containing data to program */
)
{
	ADDRESS mask = LLD_BUFFER_SIZE - 1;
	WORDCOUNT intwc = word_cnt;
	DEVSTATUS status;
	
	if (offset & mask)
	{
 		/* program only as much as necessary, so pick the lower of the two numbers */
 		if (word_cnt < (LLD_BUFFER_SIZE - (offset & mask)) ) 
    		intwc = word_cnt; 
 		else
    		intwc = LLD_BUFFER_SIZE - (offset & mask);

 		/* program the first few to get write buffer aligned */
 		status = lld_WriteBufferProgramOp(base_addr, offset, intwc, data_buf);
 		if (status != DEV_NOT_BUSY) 
   			return(status);

 		offset   += intwc; /* adjust pointers and counter */
     	word_cnt -= intwc;
     	data_buf += intwc;
     	
     	if (word_cnt == 0) 
     	{
     		return(status);
     	}
  	}

  	while(word_cnt >= LLD_BUFFER_SIZE) /* while big chunks to do */
  	{
  		//printf("base_addr = 0x%x, offset = 0x%x\n", base_addr, offset);
		status = lld_WriteBufferProgramOp(base_addr, offset, LLD_BUFFER_SIZE, data_buf);
		if (status != DEV_NOT_BUSY)
		{
			printf("status != DEV_NOT_BUSY\n"); 
  			return(status);
		}
		
		//printf("Write address: 0x%x\n", base_addr+offset);
		offset   += LLD_BUFFER_SIZE; /* adjust pointers and counter */
    	word_cnt -= LLD_BUFFER_SIZE;
    	data_buf += LLD_BUFFER_SIZE;
    	
    	//Roman add for test @2006-10-20
    	//DELAY_US(2);
  	}
  	if (word_cnt == 0) 
  	{
		return(status);
  	}

	//printf("The last, offset = %d, word_cnt = %d, data:0x%x!\n", offset, word_cnt, *data_buf);
 	status = lld_WriteBufferProgramOp(base_addr, offset, word_cnt, data_buf); 

  	return(status);
}
#endif
/******************************************************************************
* 
* lld_ReadCfiWord 
*
* RETURNS: word read
*
*/
#ifndef REMOVE_LLD_READ_CFI_WORD
FLASHDATA lld_ReadCfiWord
(
FLASHDATA * base_addr,    /* device base address is system */
ADDRESS offset        /* address offset from base address */
)
{
  FLASHDATA data;

  FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_CFI_QUERY_CMD);
  data  = FLASH_RD(base_addr, offset);
  lld_ResetCmd(base_addr);
  return(data);
}
#endif

#ifdef INCLUDE_ADVANCED_SECTOR_PROTECTION
//#include ASP	//Roman delete at 2006-10-10
#endif



/******************************************************************************
   Function : ReadFlash
   Purpose  : Read data from flash.
   Input    : pdwAddr - the address where read begin.
              ulLen   - data length will be read, by double bytes.
   Output   : pdwBuff - the buffer pointer that store the data.
   Return   : 0 - success
              others - fail
   Reference:
   Called   :

  1.Author      : Roman
    Create Date : 2006-10-11
    History     : Create function    

******************************************************************************/
int ReadFlash( ADDRESS *pdwAddr, WORDCOUNT ulLen, FLASHDATA *pdwBuff )
{
	FLASHDATA *pBaseAdd = NULL;
	FLASHDATA *pTmp = pdwBuff;	
	ADDRESS iOffset 	= 0;	
	int i = 0; 	
	
	if( ulLen <= 0 )
	{		
		return -1;
	}
		
	//Physical address translate into virtual address.
	pBaseAdd = GetVirtualMemory( pdwAddr, NULL );
	
	printf("PhyAddr:[0x%x], VirtAddr:[0x%x], length:0x%x\n", pdwAddr, pBaseAdd, ulLen);		
		
	for( iOffset = 0; iOffset < ulLen; iOffset++ )
	{
		*pTmp = lld_ReadOp( pBaseAdd, iOffset );
        
        #if 0
    	printf("%4u:%04x  ", iOffset, *pTmp);
	
		if( 0 == ( (iOffset +1 ) % 10) )
		{
			printf("\n");
		}
		#endif
		        
		pTmp++;
	}
	
	#if 0
	printf("\n");
	#endif	   
	
	return 0;
}

/******************************************************************************
   Function : WriteFlash
   Purpose  : Read data from flash.
   Input    : pdwAddr - the address where write begin.
              ulLen   - the length of data will be written into flash.
              pdwBuff - the data buffer pointer.
   Output   : 
   Return   :  0 - success
              -1 - fail
   Reference:
   Called   :

  1.Author      : Roman
    Create Date : 2006-10-11
    History     : Create function    

******************************************************************************/
int WriteFlash( ADDRESS *pdwAddr, WORDCOUNT ulLen, FLASHDATA *pdwBuff )
{
	int iRet = 0;
	FLASHDATA *pVirAdd = NULL;

	pVirAdd = (FLASHDATA *)GetVirtualMemory( (void *)pdwAddr, NULL );
	
	printf("Phy address:[0x%x], virtual address:[0x%x], ulLen = 0x%x\n", pdwAddr, pVirAdd, ulLen);

	//write file contents into flash
	if( DEV_NOT_BUSY != lld_memcpy( pVirAdd, 0, ulLen, pdwBuff) )
	{
		printf("lld_memcpy error!!\n");
		iRet = -1;
	}	

	return iRet;
}

/******************************************************************************
   Function : EraseFlash
   Purpose  : Erase a sector where the pdwAddr in.
   Input    : pdwAddr - the start address.
   Output   : none
   Return   :  0 - success
              -1 - fail
   Reference:
   Called   :

  1.Author      : Roman
    Create Date : 2006-10-11
    History     : Create function    

******************************************************************************/
int EraseFlash( ADDRESS *pdwAddr, WORDCOUNT ulLen )
{
	int iRet = 0;
	int iBlk = 0;
	unsigned long iOff = 0;
	FLASHDATA *pVirAdd = NULL;
	ADDRESS pPhyAdd = (ADDRESS)pdwAddr;
	
	//the count of sectors
	iBlk = ulLen % FLASH_SECTOR_SIZE;
	
	if( 0 == iBlk )
	{
		iBlk = (ulLen / FLASH_SECTOR_SIZE);		
	}
	else
	{
		iBlk = (ulLen / FLASH_SECTOR_SIZE)+1;
	}	
		
	//Erase all sectors within pdwAddr to pdwAddr+ulLen.
	for( iOff = 0; iOff < iBlk; iOff++ )
	{
		pVirAdd = (FLASHDATA *)GetVirtualMemory( (void *)pPhyAdd, NULL );
		//pVirAdd = GetVirtualMemory( (void *)pPhyAdd, NULL );

		printf("Erasing sector[%d]:phyaddr:0x%08x:viraddr:0x%08x \n", iOff, pPhyAdd, pVirAdd);
		if( DEV_NOT_BUSY != lld_SectorEraseOp( pVirAdd, 0 ) )
		{
			printf("Erase sector 0x%x fail!\n", pVirAdd);
			iRet = -1;
			break;
		} 
		
		pPhyAdd += FLASH_SECTOR_SIZE;
		DELAY_US(2);
	}	

	return iRet;
}


#if 0
unsigned long ReadFiletoBuf( ADDRESS *pdwAddr  )
{	
	char pFileName[] = "/usr/app/lld.exe";
	FILE *pFile = NULL;
	unsigned long ulFileSize = 0;
	unsigned long ulOffset = 0;
	FLASHDATA *pVirAdd = NULL;
	char *pBuf = NULL;
	char *pTmp = NULL;
	
	pBuf = (char *)malloc(36*1024);
	pTmp = pBuf;
			
	pFile = fopen( pFileName, "r" );
	
	if( NULL == pFile )
	{
		printf("Open file [%s] fail!\n", pFileName );
		return -1;
	}		

    //Get file property    
    pVirAdd = (FLASHDATA *)GetVirtualMemory( (void *)pdwAddr, NULL );
	
	printf("Phy address:[0x%x], virtual address:[0x%x]\n", pdwAddr, pVirAdd);

	//write file contents into flash	
	while( !feof(pFile) && !ferror(pFile) )
	{
		ulFileSize = fread( pBuf, sizeof(char), 1024, pFile );
		pBuf += ulFileSize;
		ulOffset += ulFileSize;											
	}		
	
	if( 0 == ulFileSize )
	{
		printf("Read file length isn't equal %d\n", ulFileSize );
		return -2;
	}
	
	if( DEV_NOT_BUSY != lld_memcpy( pVirAdd, 0, ulOffset, (unsigned short *)pTmp) )
	{
		printf("lld_memcpy error!!\n");
	}				
	
	if( pTmp )
	{
		free( pTmp );
		pTmp = NULL;
	}
	
	fclose( pFile );
	
	return ulFileSize;
}

int WriteBuftoFile( FLASHDATA *szBuf, int iLen )
{
	char pFileName[] = "/usr/lib/v100/res/fpga/v100_cpu.bin";
	FILE *pFile = NULL;
		
	pFile = fopen( pFileName, "r+" );
	
	if( NULL == pFile )
	{
		printf("Open file [%s] fail!\n", pFileName );
		return -1;
	}
	
	if( iLen != fwrite( szBuf, sizeof(char), iLen, pFile ) )
	{
		printf("Read file length isn't equal %d\n", iLen );
		return -2;
	}
	
	fclose( pFile );
	
	return 0;
}

int ReadFlashtoBuf( FLASHDATA *pVirAdd, FLASHDATA *szBuf, long iLen )
{		
	FLASHDATA *pBuf		= NULL;
	long iOffset 		= 0;
	FLASHDATA usResult 	= 0;
	
	printf("Into ReadFlashtoBuf...\n");
	if( NULL == szBuf )
	{
		return -1;
	}
	
	pBuf = szBuf;
	
	printf("step 2..\n");
		
	for( iOffset = 0; iOffset < (iLen/2); iOffset++ )
	{
		//printf("step iOffset = %d, pVirAdd = 0x%x..\n", iOffset, pVirAdd );		
		usResult = lld_ReadOp( pVirAdd, iOffset );
		
		//printf("after lld_ReadOp...\n");		
		*pBuf = usResult;		
		pBuf += 2;
	}
	
	return 0;
}

#endif



