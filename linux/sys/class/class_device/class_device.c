/*======================================================================
    A simple kernel module: "hello world"
         
    The initial developer of the original code is Baohua Song
    <author@linuxdriver.cn>. All Rights Reserved.
======================================================================*/
#include <linux/init.h>
#include <linux/module.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>


#define NUMBER 256
#define MEM_SIZE 2048
#define MEM_CLEAR 1


static int dev_major = 0;

static struct cdev mem_dev;
static char mem[MEM_SIZE];
static struct class *my_class;
static struct device *my_device;


static int memdev_read(struct file *filp, char __user *buf, size_t nbuf, loff_t *offs)
{
	unsigned long p = *offs;
	unsigned int count = nbuf;
	int ret = 0;

	//分析和获取有效长度
	if(p > MEM_SIZE)
		return count ? - ENXIO: 0;
	if(count > MEM_SIZE - p)
		count = MEM_SIZE - p;	

	ret = copy_to_user(buf, mem + p,  count);
	if (ret) {
		printk(KERN_INFO "copy_to_user failed!\n");	
	} else {
		*offs += count;
		ret = count;
		printk(KERN_INFO "copy %d bytes from %ld\n", count, p);	
	}

	
	return ret;
}

static int memdev_write(struct file *filp, const char __user *buf, size_t nbuf, loff_t *offs )
{
	unsigned long p = *offs;
	unsigned int count = nbuf;
	int ret = 0;

	//分析和获取有效长度
	if(p > MEM_SIZE)
		return count ? - ENXIO: 0;
	if(count > MEM_SIZE - p)
		count = MEM_SIZE - p;	

	ret = copy_from_user(mem + p, buf, count);
	if (ret) {
                printk(KERN_INFO "copy_from_user failed!\n");
        } else {
                *offs += count;
                ret = count;
                printk(KERN_INFO "write %d bytes to %ld\n", count, p);
        }

	return ret;
}

//seek 文件定位函数
static loff_t memdev_lseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;

	switch(orig) {
                //相对文件开始位置偏移
		case 0: 
			if(offset < 0) {
				ret = -EINVAL;
				break;
			}
			if((unsigned int)offset > MEM_SIZE) {
				ret = -EINVAL;
				break;
			}
			filp->f_pos = (unsigned int)offset;      //filp->f_pos 表示文件当前指针位置
			ret = filp->f_pos;
			break;
		//相对文件当前位置偏移
		case 1:
			if(filp->f_pos + offset > MEM_SIZE) {
				ret = -EINVAL;
				break;
			}
			if(filp->f_pos + offset < 0) {
				ret = -EINVAL;
				break;
			}
			filp->f_pos += offset;
			ret = filp->f_pos;
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

static int memdev_open(struct inode *inode, struct file *filp)
{

	return 0;
}

static int memdev_ioctl(struct inode *inodep, struct file *filp, 
				unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case MEM_CLEAR:
			memset(mem, 0, MEM_SIZE);
			printk(KERN_INFO "mem has set to zero!\n");
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

static int memdev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations memdev_ops = {
	.owner = THIS_MODULE,
	.read = memdev_read,
	.write = memdev_write,
	.llseek = memdev_lseek,
	.open = memdev_open,
	.ioctl = memdev_ioctl,
	.release = memdev_release,
};

static int class_device_init(void)
{
	int err;
	dev_t devno;


	printk(KERN_INFO "%s\n", __func__);

	err = alloc_chrdev_region(&devno, 0, 1, "mem_dev");  //注册成功返回0
	if (err) {
		printk(KERN_INFO "alloc char device failed!\n");
		return err;
	}
	dev_major = MAJOR(devno);

	cdev_init(&mem_dev, &memdev_ops);	

	err = cdev_add(&mem_dev, devno, 1);  //添加字符设备成功返回0
	if (err) {
		printk(KERN_INFO "add char device failed!\n");
		unregister_chrdev_region(devno, 1);
		return err;
	}

        //create "my_class" directory under /sys/class
	my_class = class_create(THIS_MODULE, "my_class");
	if(IS_ERR(my_class)) {
		printk(KERN_INFO "class create failed!\n");
		my_class = NULL;
	}

	//create "my_device" directory under /sys/class/my_class
	my_device = device_create(my_class, NULL, devno, NULL, "my_device");
	if(IS_ERR(my_device)) {
		printk("device_create() returned error %ld\n", PTR_ERR(my_device));
		my_device = NULL;
	}

	//初始化mem buffer
	memset(mem, 0, MEM_SIZE);

	return 0;
}

static void class_device_exit(void)
{
	cdev_del(&mem_dev);

	unregister_chrdev_region(MKDEV(dev_major, 0), 1);

	if(NULL != my_class) {
		if (NULL != my_device) {
			device_destroy(my_class, MKDEV(dev_major, 0));
		}
		class_destroy(my_class);
	}

	printk(KERN_INFO " Hello World exit\n ");
}

module_init(class_device_init);
module_exit(class_device_exit);

MODULE_AUTHOR("Song Baohua");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");
