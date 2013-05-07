#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>	/* for accessing user-space */
#include "ringbuffer.h"

static unsigned major = 0;				/* save major number assigned to driver */
static struct cdev mydev;				/* to be registered with kernel */
static struct class *mydev_class = NULL;		/* device class to register with sysfs */
static struct device *mydev_device;			/* device registered with sysfs */

#define MYDEV_RING_SIZE 2048					/* accomodate 2K per ring */
static struct OutRingBuffer *mydev_ring;			/* ring buffer handle */
static char rbuf[MYDEV_RING_SIZE], wbuf[MYDEV_RING_SIZE];	/* temporary buffers for moving data
								   between user-space and device */

static ssize_t
mydev_read(struct file *filp, char __user *buf, size_t nbuf, loff_t *offs)
{
	/* cap nbuf to MYDEV_RING_SIZE; otherwise we risk overflowing our tmp buffer */
	if (nbuf > MYDEV_RING_SIZE)
		nbuf = MYDEV_RING_SIZE;

	/* read data from device into the tmp buffer, since device cannot write directly
	   to user-space memory */
	nbuf = rb_read(mydev_ring, rbuf, nbuf);

	/* copy the data from the user-supplied buffer to the tmp buffer, since device cannot
	   directly read from user-space memory */
	copy_to_user(buf, rbuf, nbuf);

	/* adjust file offset */
	*offs += nbuf;

	/* return number of bytes read */
	return nbuf;
}

static ssize_t
mydev_write(struct file *filp, const char __user *buf, size_t nbuf, loff_t *offs)
{
	/* cap nbuf to MYDEV_RING_SIZE; otherwise we risk overflowing our tmp buffer */
	if (nbuf > MYDEV_RING_SIZE)
		nbuf = MYDEV_RING_SIZE;

	/* copy the data from the user-supplied buffer to the tmp buffer, since device cannot
	   directly read from user-space memory */
	copy_from_user(wbuf, buf, nbuf);

	/* write data out to device */
	nbuf = rb_write(mydev_ring, wbuf, nbuf);

	/* adjust file offset */
	*offs += nbuf;

	/* return number of bytes written */
	return nbuf;
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
		/* Dynamically allocate a major/minor number */
		rc = alloc_chrdev_region(&devid, 0, 1, "mydev");

		/* save major number component of devid */
		major = MAJOR(devid);
	}

	if (rc) {
		/* inability to register major number is fatal */
		printk("Trouble registering major number!\n");
		return rc;
	}

	/* Time to initialize hardware, for this must be done prior to calling
	   cdev_add(), after which time requests can start coming in */

	rc = rb_init(&mydev_ring, MYDEV_RING_SIZE);

	if (rc) {
		printk("rb_init() returned error %d\n", rc);
		unregister_chrdev_region(devid, 1);
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
		rb_destroy(mydev_ring);
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

	/* Destroy ring buffers only after cdev interfaces is brought down */
	rb_destroy(mydev_ring);

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
