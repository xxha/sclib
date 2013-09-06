#include <asm/io.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
MODULE_LICENSE("Dual BSD/GPL");

#include "sti7105.h"
#include "stsys.h"
#include "stddefs.h"
#include "sti2c.h"
#include "stsys.h"
#define I2C1 0x01
//extern U32 at24c256_i2c1_cfg(void);
ST_DeviceName_t PIO_DeviceName[] = {"PIO0","PIO1","PIO2","PIO3","PIO4","PIO5", \
				"PIO6","PIO7","PIO8","PIO9","PIO10","PIO11", \
				"PIO12","PIO13","PIO14","PIO15","PIO16","PIO17", \
				"PIO18","PIO19","PIO20","PIO21","PIO22","PIO23", \
				"PIO24","PIO25","PIO26"};
static STI2C_Handle_t I2c_handle;
static U8 init_data[] =
{
	1, 0x00,
	2, 0xc0,
	3, 0x0a,
	4, 0xab,
	5, 0x08,
	6, 0x88,
	7, 0x00,
	8, 0xcd,
	9, 0x0f,
	10, 0xf0,
	11, 0x7f,
};
U32 static ee_i2c_write(U16 sub_addr,U8 *data,U16 data_size)
{
	
	STI2C_Params_t I2C_Params;
	U32 ErrCode;
	U32 nread ,nwrite;
	U8 buf_write[64];
// U8 buf_read[64];
// U32 i;
	if (NULL == buf_write)
	{
		return -1;
	}
	buf_write[0] = (sub_addr>>8)&0xff;
	buf_write[1] = sub_addr&0xff;
	memcpy(&buf_write[2],data,data_size);
	I2C_Params.I2cAddress = 0xA0;
	I2C_Params.AddressType      = STI2C_ADDRESS_7_BITS;
	I2C_Params.BaudRate         = STI2C_RATE_NORMAL;
	I2C_Params.BusAccessTimeOut = 100000;
			     ErrCode = STI2C_SetParams(I2c_handle, &I2C_Params);
	if(ST_NO_ERROR != ErrCode)
	{
		printk("I2C set params error:%u\n",ErrCode);
		return -1;
	}
	printk("I2C1 set params success!\n");
	
	/* write data */
	ErrCode = STI2C_Write(I2c_handle, &buf_write[0], data_size+2, 100, &nwrite);
	if(ST_NO_ERROR != ErrCode )
	{
		printk("I2C write addr error : 0x%x\n",ErrCode);
		return ErrCode;
	} 
	
	mdelay(100);
	printk("I2C write sucess!\n");
	
	/*read data*/
/* ErrCode = STI2C_WriteNoStop(I2c_handle, &buf_write[0], 2, 100, &nwrite);
	if(ST_NO_ERROR != ErrCode )
	{
		printk("I2Ci_read write addr error : 0x%x\n",ErrCode);
		return ErrCode;
	}
	printk("I2C_read write address success!\n");
	ErrCode = STI2C_Read(I2c_handle, &buf_read, data_size, 100, &nread);
	if(ST_NO_ERROR != ErrCode )
	{
		printk("I2C read data error : 0x%x\n",ErrCode);
		return ErrCode;
	}
	
	for(i=0;i<data_size;i++)
	{
		printk("buf_read[%d] = 0x%x\n",i,buf_read[i]);
	}
*/
	return ST_NO_ERROR;
}
static U32 ee_i2c_read(U16 sub_addr,U8 *data,U16 data_size)
{
	
	U32 i;
	U32 ErrCode;
	U32 nwrite,nread;
	U8 buf_write[2];
		
	buf_write[0] = (sub_addr>>8)&0xff;
	buf_write[1] = sub_addr&0xff;
	/*read data*/
	ErrCode = STI2C_WriteNoStop(I2c_handle, &buf_write[0], 2, 100, &nwrite);
	if(ST_NO_ERROR != ErrCode )
	{
		printk("I2Ci_read write addr error : 0x%x\n",ErrCode);
		return ErrCode;
	}
	printk("I2C_read write address success!\n");
	ErrCode = STI2C_Read(I2c_handle, data, data_size, 100, &nread);
	if(ST_NO_ERROR != ErrCode )
	{
		printk("I2C read data error : 0x%x\n",ErrCode);
		return ErrCode;
	}
	
/* for(i=0;i<data_size;i++)
	{
		printk("buf_read[%d] = 0x%x\n",i,buf_read[i]);
	}
*/
	return ST_NO_ERROR;
}
U32 static reg_check(void)
{
	U32 i;
	U32 nwrite,nread;
	U32 ErrCode;
	U8 temp_buf[30];
	for(i = 0; i < sizeof(init_data); i += 2)
	{
		ErrCode = STI2C_WriteNoStop(I2c_handle,&init_data[i],1,100,&nwrite);
		if(ST_NO_ERROR != ErrCode)
		{
			printk("reg_dmup: I2C write addr error:0x%x\n",ErrCode);
			return ErrCode;
		}
		
		ErrCode = STI2C_Read(I2c_handle,&temp_buf[i + 1],1,100,&nread);
		if(ST_NO_ERROR != ErrCode)
		{
			printk("reg_dmup: I2C write data error:0x%x\n",ErrCode);
			return ErrCode;
		}
		printk("temp_buf[%d] = 0x%x\n",i+1,temp_buf[i+1]);
	}

}
void at24c_init(void)
{
	STI2C_InitParams_t I2C_InitParams;
	STI2C_OpenParams_t I2C_OpenParams;
	U32 ErrCode;
	U8 buf1[64];
	U32 i;

	STSYS_WriteRegDev32LE(ST7105_CFG_BASE_ADDRESS+0x140/*SYSTEM_CONFIG16*/,0x00050a00);
	STSYS_WriteRegDev32LE(ST7105_CFG_BASE_ADDRESS+0x164/*SYSTEM_CONFIG21*/,0x0df0);

	memset(&I2C_InitParams,0,sizeof(STI2C_InitParams_t));
	I2C_InitParams.BaseAddress         = (U32 *)ST7105_SSC2_BASE_ADDRESS;
	I2C_InitParams.InterruptNumber     = ST7105_SSC2_INTERRUPT;
// I2C_InitParams.InterruptLevel      = SSC_2_INTERRUPT_LEVEL;
	I2C_InitParams.MasterSlave         = STI2C_MASTER;
	I2C_InitParams.BaudRate            = STI2C_RATE_NORMAL;
	I2C_InitParams.MaxHandles          = 8;
	I2C_InitParams.ClockFrequency      = 0x5f5e100;
// I2C_InitParams.DriverPartition     = cache_partition_sdk[0];
	I2C_InitParams.PIOforSDA.BitMask   = PIO_BIT_5;
	I2C_InitParams.PIOforSCL.BitMask   = PIO_BIT_4;
	I2C_InitParams.EvtHandlerName[0]   = '\0';
	I2C_InitParams.GlitchWidth         = 0;
	I2C_InitParams.SlaveAddress        = 0;
	I2C_InitParams.FifoEnabled         = FALSE;
	strcpy(I2C_InitParams.PIOforSDA.PortName,PIO_DeviceName[3]);
	strcpy(I2C_InitParams.PIOforSCL.PortName,PIO_DeviceName[3]);

	ErrCode=STI2C_Init("I2C1",&I2C_InitParams);
	if (ST_NO_ERROR != ErrCode )
	{
		printk("I2C1 Init ERROR : %u\n",ErrCode);
		return;
	}
	printk("\nI2C1 Init success.\n");
	memset(&I2C_OpenParams,0,sizeof(STI2C_OpenParams_t));
	I2C_OpenParams.BusAccessTimeOut = 100000;
	I2C_OpenParams.AddressType      = STI2C_ADDRESS_7_BITS;
	I2C_OpenParams.I2cAddress       = 0xA0;
	I2C_OpenParams.BaudRate         = STI2C_RATE_NORMAL;
	ErrCode=STI2C_Open("I2C1",&I2C_OpenParams, &I2c_handle);//Ii2c_handle point to "I2C1" 
	if (ST_NO_ERROR != ErrCode )
	{
		printk("<1>  i2c1 open error: %u\n", ErrCode);
		printk("<2> I2cAddress is %x\n",I2C_OpenParams.I2cAddress);
		return ;
	} 
	
	
// printk("I2c_handle =0x%x\n",I2c_handle);
	printk("I2C1 open success! \ni2c_hamdle = 0x%x\n",I2c_handle);
	
	ee_i2c_write(0x01,&init_data,sizeof(init_data));
	ee_i2c_read(0x01,&buf1,sizeof(buf1));
	for(i=0;i<sizeof(buf1);i++)
	{
		printk("buf_read[%d] = 0x%x\n",i,buf1[i]);
	}
// chip_write();
// chip_read()
// reg_check();
	return ;
}
void at24c_exit(void)
{
	printk("remove at24c\n");
}
//EXPORT_SYMBOL(main);
module_init(at24c_init);
module_exit(at24c_exit);
