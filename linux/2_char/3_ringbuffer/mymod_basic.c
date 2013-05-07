#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static unsigned major = 0;				/* save major number assigned to driver */
static struct cdev mydev;				/* to be registered with kernel */
static struct class *mydev_class = NULL;		/* device class to register with sysfs */
static struct device *mydev_device;			/* device registered with sysfs */

static ssize_t
mydev_read(struct file *filp, char __user *buf, size_t nbuf, loff_t *offs)
{
	return 0;
}

static ssize_t
mydev_write(struct file *filp, const char __user *buf, size_t nbuf, loff_t *offs)
{
	return 0;
}

static const struct file_operations mydev_fops = {
	.owner = THIS_MODULE,
	.read = mydev_read,
	.write = mydev_write
};

module_param(major, uint, 0);		/* allow manual specification of major # */

static int __init
mymod_init (void)
{
	int rc;
	dev_t devid;

	/* Handle static allocation of major number (specified as a module parameter) or
	   otherwise dynamically allocate the major number */

	if (major) {
		/* Use the major number specified */
		devid = MKDEV(major, 0);
		rc = register_chrdev_region(devid, 1, "mydev");
	} else {
		/* Dynamically allocate a range of ndev major/minor numbers */
		rc = alloc_chrdev_region(&devid, 0, 1, "mydev");

		/* save major number component of devid */
		major = MAJOR(devid);
	}

	if (rc) {
		/* inability to register major number is fatal */
		printk("Trouble registering major number!\n");
		return rc;
	}

	/* Initalize the struct cdev structure, and set the table of file
	   operation functions (this function cannot fail) */

	cdev_init(&mydev, &mydev_fops);

	/* Register the char device with the kernel. At this point, driver
	   should be prepared to handle requests (particularly if the device
	   nodes were already created) */

	if ((rc = cdev_add(&mydev, devid, 1))) {
		/* failure at this point is fatal */
		printk("Trouble registering with kernel!\n");
		unregister_chrdev_region(devid, 1);
		return rc;
	}

	/* Establish a class under which we will register our driver with sysfs.
	   Registration with sysfs is nice, but not necessary for the driver to
	   work. Therefore, do not consider failure at this point to be fatal. */
		   
	mydev_class = class_create(THIS_MODULE, "mydev");

	if (IS_ERR(mydev_class)) {
		printk("class_create() returned error %ld\n", PTR_ERR(mydev_class));
		mydev_class = NULL;
	} else {
		/* Register device with sysfs. Among other things, this
		   will trigger creation of the device node in /dev */

		mydev_device = device_create(mydev_class, NULL, devid, NULL, "mydev0");
		if (IS_ERR(mydev_device)) {
			printk("device_create() returned error %ld\n", PTR_ERR(mydev_device));
			mydev_device = NULL;
		}
	}

	printk("Hello from mydev!\n");
	return 0;
}  

static void __exit
mymod_exit(void) 
{
	/* deregister from kernel */
	cdev_del(&mydev);

	/* release major/minor numbers allocated to the driver */
	unregister_chrdev_region(MKDEV(major, 0), 1);

	if (mydev_class != NULL) {
		/* deregister device from sysfs */
		device_destroy(mydev_class, MKDEV(major, 0));

		class_destroy(mydev_class);
	}

	printk("Goodbye from mydev.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Character driver skeleton.");
