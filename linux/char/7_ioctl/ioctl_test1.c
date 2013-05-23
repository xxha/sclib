#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <linux/errno.h>        /* error codes */
#include <linux/cdev.h>
#include <linux/fs.h>           /* everything... */

#include <asm/uaccess.h>        /* access_ok() */

/*
 * Ioctl definitions
 */
#define TEST_IOC_MAGIC 'n'

#define ZERO    _IO(TEST_IOC_MAGIC, 0)
#define FIRST   _IO(TEST_IOC_MAGIC, 1)

#define SECOND  _IOR(TEST_IOC_MAGIC, 2, int)
#define THIRD   _IOR(TEST_IOC_MAGIC, 3, struct read_struct)

#define FOURTH   _IOW(TEST_IOC_MAGIC, 4, int)
#define FIFTH    _IOW(TEST_IOC_MAGIC, 5, unsigned char)

#define SIXTH    _IOWR(TEST_IOC_MAGIC, 6, int)
#define SEVENTH  _IOWR(TEST_IOC_MAGIC, 7, unsigned char)

#define EIGHTH   _IO(TEST_IOC_MAGIC, 8)
#define NINTH    _IO(TEST_IOC_MAGIC, 9)
#define TENTH    _IO(TEST_IOC_MAGIC, 10)

#define TEST_IOC_MAXNR  10


/*
 * Device parameters
 */
#define DEVICE_MAJOR 0
#define DEVICE_NUMBER 1
#define DEVICE_MINOR 0

static int dev_major = DEVICE_MAJOR;
static int dev_num = DEVICE_NUMBER;
static int dev_minor = DEVICE_MINOR;

/*
 * cdev related
 */
struct cdev *test_cdev;

static int read_num = 1111;
//static char *read_string = "abcd";

struct read_struct {
	char read_string;
	int read_n;
};

struct read_struct *read_struct1;

static int write_num = 0;
static char *write_string = NULL;
static int rdwr_num = 0;
static char *rdwr_str = NULL;

int test_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "%s\n", __func__);

	filp->private_data = inode->i_cdev;
	return 0;
}

int test_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

int test_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg)
{

	int ret = 0;
	int err = 0;
	printk(KERN_INFO "%s\n", __func__);

	if (_IOC_TYPE(cmd) != TEST_IOC_MAGIC) {
		printk(KERN_INFO "IOC cmd type error\n");
		return -ENOTTY;
	}

	if (_IOC_NR(cmd) > TEST_IOC_MAXNR) {
		printk(KERN_INFO "IOC cmd number error\n");
		return -ENOTTY;
	}

        if (_IOC_DIR(cmd) & _IOC_READ)
                err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
        else if (_IOC_DIR(cmd) & _IOC_WRITE)
                err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
        if (err) {
		printk(KERN_INFO "access arg error\n");
		return -EFAULT;
	}

	switch (cmd) {
		case ZERO:
			printk(KERN_INFO "ioctl 0.\n");
			break;
		case FIRST:
			printk(KERN_INFO "ioctl 1.\n");
			break;
		case SECOND:
			__put_user(read_num, (int __user *)arg);
			printk(KERN_INFO "ioctl 2, read_num = %d.\n", read_num);
			break;
		case THIRD:
			copy_to_user((void __user *)arg, (const void *)read_struct1, sizeof(struct read_struct));
			//__put_user(*read_struct1, (struct read_struct __user *)arg);
			printk(KERN_INFO "ioctl 3, read_struct1->read_string = %c\n", read_struct1->read_string);
			printk(KERN_INFO "ioctl 3, read_struct1->read_n = %d\n", read_struct1->read_n);
			break;
		case FOURTH:
//			printk(KERN_INFO "ioctl 4, old write_num = %d\n", write_num);
//			__get_user(write_num, (int __user *)arg);
//			printk(KERN_INFO "ioctl 4, new write_num = %d\n", write_num);
			break;
		case FIFTH:
//			__get_user(*write_string,(unsigned char __user *)arg);
//			printk(KERN_INFO "ioctl 5, write_string = %s\n", write_string);
			break;
		case SIXTH:
/*			ret = __get_user(rdwr_num, (int __user *)arg);
			if(ret == 0) {
				ret = __put_user(rdwr_num, (int __user*)arg);
				if (ret == 0)
					printk(KERN_INFO "write read num succeed.\n");
			} 
*/			printk(KERN_INFO "ioctl 6.\n");
			break;
		case SEVENTH:
			ret = __get_user(*rdwr_str, (unsigned char __user *)arg);
			if(ret == 0) {
				ret = __put_user(*rdwr_str, (unsigned char __user*)arg);
				if (ret == 0)
					printk(KERN_INFO "write read char succeed.\n");
			} 
			printk(KERN_INFO "ioctl 7.\n");
			break;
		case EIGHTH:
			printk(KERN_INFO "ioctl 8.\n");
			break;
		case NINTH:
			printk(KERN_INFO "ioctl 9.\n");
			break;
		case TENTH:
			printk(KERN_INFO "ioctl 10.\n");
			break;

	}
	return 0;
}

struct file_operations test_fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.ioctl = test_ioctl,
	.release = test_release,
};


int test_ioctl_init(void)
{
	dev_t dev = 0;
	int err;
	printk(KERN_INFO "%s\n", __func__);

	read_struct1 = kzalloc(sizeof(struct read_struct), GFP_KERNEL);
	if(!read_struct1){
		printk(KERN_INFO "kzalloc read_struct1 failed.\n");
	}
/*
	read_struct1->read_string = kzalloc(8, GFP_KERNEL);
	if(!(read_struct1->read_string)){
		printk(KERN_INFO "kzalloc read_struct1->read_string failed.\n");
	}
*/	
	read_struct1->read_string = 'c';
	read_struct1->read_n = 3333;


	if (dev_major) {
		printk(KERN_INFO "static register char device.\n");
		dev = MKDEV(dev_major, 0);
		err = register_chrdev_region(dev, dev_num, "ioctl_test");
	} else {
		printk(KERN_INFO "dynamic alloc char device.\n");
		err = alloc_chrdev_region(&dev, dev_minor, dev_num, "ioctl_test");
		dev_major = MAJOR(dev);
	}

	if (err < 0) {
		printk(KERN_INFO "ioctl register char device failed.\n");
		return err;
	}

	test_cdev = kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if(!test_cdev) {
		err = -ENOMEM;
		printk(KERN_INFO "kmalloc test_cdev failed.\n");
		return err;
	}

	/*add char devices*/
	cdev_init(test_cdev, &test_fops);

	test_cdev->owner = THIS_MODULE;
	test_cdev->ops = &test_fops;

	err = cdev_add(test_cdev, dev, 1);
	if(err)
		printk(KERN_INFO "add test_cdev to cdev failed.\n");

	return 0;
}

void test_ioctl_exit(void)
{
	dev_t dev = MKDEV(dev_major, dev_minor);

	cdev_del(test_cdev);
	kfree(test_cdev);
	unregister_chrdev_region(dev, dev_num);
}

module_init(test_ioctl_init);
module_exit(test_ioctl_exit);


