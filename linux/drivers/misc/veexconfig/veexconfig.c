#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/crc32.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/miscdevice.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <asm/system.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/irq.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/clk.h>

/* Memory mapping definitions */
#define MAP_SIZE 4*1024
#define MAP_MASK (MAP_SIZE - 1)

#define CONFIG_BASE 			0xD8002000

#define	CPLD_MAP_SIZE			0x30
#define CPLD_BASE_ADDR			0xF4000000

#define	CPLD_OFFSET_REG1		0x00
#define	CPLD_OFFSET_REG2		0x01
#define	CPLD_OFFSET_REG3		0x02
#define	CPLD_OFFSET_REG4		0x03

#define	CPLD_OFFSET_ID1			0x08
#define	CPLD_OFFSET_ID2			0x09
#define	CPLD_OFFSET_ID3			0x0A
#define	CPLD_OFFSET_ID4			0x0B
#define	CPLD_OFFSET_HVER		0x0C
#define	CPLD_OFFSET_CVER		0x0D
#define	CPLD_OFFSET_STAT		0x0E

#define	CPLD_INT			gpio_to_irq(6*32 + 4)


#define FEC_EVENT_MII			0x00800000

/* the defins of MII operation */ 
#define FEC_MII_ST			0x40000000
#define FEC_MII_OP_OFF			28
#define FEC_MII_OP_MASK 		0x03
#define FEC_MII_OP_RD			0x02
#define FEC_MII_OP_WR			0x01
#define FEC_MII_PA_OFF			23
#define FEC_MII_PA_MASK 		0xFF
#define FEC_MII_RA_OFF			18
#define FEC_MII_RA_MASK			0xFF
#define FEC_MII_TA			0x00020000
#define FEC_MII_DATA_OFF 		0
#define FEC_MII_DATA_MASK 		0x0000FFFF

#define FEC_MII_FRAME	( FEC_MII_ST | FEC_MII_TA )	
#define FEC_MII_OP(x)	( ((x) & FEC_MII_OP_MASK) << FEC_MII_OP_OFF )
#define FEC_MII_PA(pa)  (((pa)& FEC_MII_PA_MASK) << FEC_MII_PA_OFF)
#define FEC_MII_RA(ra)	(((ra)& FEC_MII_RA_MASK) << FEC_MII_RA_OFF)
#define FEC_MII_SET_DATA(v) (((v) & FEC_MII_DATA_MASK) << FEC_MII_DATA_OFF)
#define FEC_MII_GET_DATA(v) (((v) >> FEC_MII_DATA_OFF) & FEC_MII_DATA_MASK )
#define FEC_MII_READ(pa, ra) (	( FEC_MII_FRAME | FEC_MII_OP(FEC_MII_OP_RD) )|\
					FEC_MII_PA(pa) | FEC_MII_RA(ra) )
#define FEC_MII_WRITE(pa, ra, v) ( FEC_MII_FRAME | FEC_MII_OP(FEC_MII_OP_WR)|\
				FEC_MII_PA(pa) | FEC_MII_RA(ra) |FEC_MII_SET_DATA(v) )

#define MII_SPEED_SHIFT			1
#define MII_SPEED_MASK 			0x0000003F
#define MII_SPEED(x)			( (((((x)+499999)/2500000)&(MII_SPEED_MASK))>>1)<<(MII_SPEED_SHIFT) )

#define FEC_MII_TICK			2
#define FEC_MII_TIMEOUT			(1000*1000)

#define	FEC_MII_EIR_OFFSET		0x04
#define	FEC_MII_MMFR_OFFSET		0x40

#define	FEC_MAC_LOW			0xE4
#define	FEC_MAC_HIGH			0xE8

#define	MII_READ		0x01
#define	MII_WRITE		0x02
#define	MII_PHYADDR_SET		0x03

#define	V100_SET_16BIT		0x10
#define	V100_SET_8BIT		0x11

#define CONT_REG2 			0x01
#define KEY_PW2SEC			0x04
#define SD_EN 				(0x1<<14)
#define BUZZER_EN  			0x4
#define POWEROFF 			0x2

volatile void * CONFIG_MAPPED_BASE = NULL;
volatile unsigned short * cpld_mapped_base = NULL;

typedef struct veex_ioctl_data {
	unsigned char reg;
	unsigned short data;
}veex_ioctl_data;

unsigned char phy_addr = 0;

static struct fasync_struct *config_async_queue;
struct timer_list       poweroff_timer;

#ifdef MDIO_ACCESS
extern int mxc_fec_mii_write(unsigned char phy_addr, unsigned char reg_addr, unsigned short * value);
extern int mxc_fec_mii_read(unsigned char phy_addr, unsigned char reg_addr, unsigned short * value);
#endif

void beep(unsigned char on)
{
	if(on > 0x01) return;

	if(on)
		*(unsigned short *)(cpld_mapped_base) |= BUZZER_EN;
	else
		*(unsigned short *)(cpld_mapped_base) &= ~BUZZER_EN;
}

static int config_ioctl(struct inode *inode, struct file *file,
		     unsigned int cmd, unsigned long arg)
{
	struct veex_ioctl_data data;

	switch (cmd) {
#ifdef MDIO_ACCESS
		case MII_READ:
			if (copy_from_user(&data, (struct veex_ioctl_data *)arg, sizeof(veex_ioctl_data))){
				printk("copy from user failed!\n");
    				return -EFAULT;
     			}

			ret = mxc_fec_mii_read(phy_addr, data.reg, &data.data);
			if(ret < 0){
				printk("mii read [0x%x] failed!\n", data.reg);
				return -EFAULT;
			}

			if (copy_to_user((struct veex_ioctl_data *)arg, &data, sizeof(veex_ioctl_data))){
				printk("copy to user failed!\n");
		    		return -EFAULT;
		     	}

			break;
		case MII_WRITE:

			if (copy_from_user(&data, (struct veex_ioctl_data *)arg, sizeof(veex_ioctl_data))){
				printk("copy from user failed!\n");
		    		return -EFAULT;
		     	}

			ret = mxc_fec_mii_write(phy_addr, data.reg, &data.data);
			if(ret < 0){
				printk("mii write [0x%x] failed!\n", data.reg);
				return -EFAULT;
			}

			break;
			
		case MII_PHYADDR_SET:

			if (copy_from_user(&data, (struct veex_ioctl_data *)arg, sizeof(veex_ioctl_data))){
				printk("copy from user failed!\n");
		    		return -EFAULT;
		     	}
			
			phy_addr = data.reg;
			
			break;
#endif
			
		case V100_SET_16BIT:

			if (copy_from_user(&data, (struct veex_ioctl_data *)arg, sizeof(veex_ioctl_data))){
				printk("copy from user failed!\n");
    				return -EFAULT;
     			}

#if 1
/*
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x40) = 0x3000DE78;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x44) = 0xFFFF0D03;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x48) = 0xFFFF0900;
*/
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x40) = 0x3000DE70;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x44) = 0x00000503;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x48) = 0x00E00008;
			
#else
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x40) = 0x3000CE70;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x44) = 0x00000503;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x48) = 0x00000008;
#endif	

			break;
			
		case V100_SET_8BIT:

			if (copy_from_user(&data, (struct veex_ioctl_data *)arg, sizeof(veex_ioctl_data))){
				printk("copy from user failed!\n");
    		return -EFAULT;
     	}

			*(unsigned int *)(CONFIG_MAPPED_BASE+0x40) = 0x3000DF7F;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x44) = 0xFFFF0B03;
			*(unsigned int *)(CONFIG_MAPPED_BASE+0x48) = 0xFFFF0900; 

			break;
		
		default:
				printk("Unknow command\n");
			break;
	}
	
	return 0;
}

static int config_open(struct inode *inode, struct file *file)
{

	return 0;
}

static int config_release(struct inode *inode, struct file *file)
{

	return 0;
}

static int config_fasync(int fd, struct file *file, int on)
{
	return fasync_helper(fd, file, on, &config_async_queue);
}

static struct file_operations config_fops = {
	.open		= config_open,
	.release	= config_release,
	.ioctl  	= config_ioctl,
	.fasync		= config_fasync,
};

static struct miscdevice config_dev = {
	.minor  = 199,
	.name   = "veexconfig",
	.fops   = &config_fops,
};

static void powercut(unsigned long data)
{
	if((*((unsigned short *)(cpld_mapped_base+KEY_PW2SEC)) & 0x1 ) == 0x1){
		printk(KERN_EMERG "Force power off found, shutdown the SD card controller...");
		printk(KERN_EMERG "Done!!!\n");
		*(unsigned short *)(cpld_mapped_base + CONT_REG2) &= ~SD_EN;
		mdelay(500);
		*(unsigned short *)(cpld_mapped_base) |= POWEROFF;
	}
}

static unsigned int  cpld_irq_handler(u32 irq, struct irq_desc *desc)
{
	kill_fasync(&config_async_queue, SIGIO, POLL_IN);

	beep(1);
	mdelay(250);
	beep(0);
	mdelay(250);
	beep(1);
	mdelay(250);
	beep(0);

	init_timer(&poweroff_timer);
	poweroff_timer.function = powercut;
	poweroff_timer.expires = jiffies + HZ*4;
	add_timer(&poweroff_timer);

	return IRQ_HANDLED;
}

static int __init veex_config_init(void)
{
	int ret = 0;

	/*
	 * addr * Map the attribute space.  This is overkill, but clean.
	 * clean */
	cpld_mapped_base = ioremap_nocache(CPLD_BASE_ADDR, CPLD_MAP_SIZE);
	if (!cpld_mapped_base)
		return -ENOMEM;

	set_irq_type(CPLD_INT, IRQF_TRIGGER_FALLING);
	ret = request_irq(CPLD_INT, (void *) &cpld_irq_handler, 0, "CPLD_IRQ", 0);
	if( ret < 0 )
		return -ENOMEM;

	misc_register(&config_dev);

	return 0;
}

static void __exit veex_config_exit(void)
{
	misc_deregister(&config_dev);
	iounmap((void *)cpld_mapped_base);

	free_irq(CPLD_INT, 0);
}



MODULE_AUTHOR("Yanjun Luo <yluo@vetronicsltd.com.cn>");
MODULE_DESCRIPTION("VEEX config driver");
MODULE_LICENSE("GPL");

module_init(veex_config_init);
module_exit(veex_config_exit);

