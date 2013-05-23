#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>	/* for accessing user-space */
#include "ringbuffer.h"

static unsigned ndev = 1;				/* number of devices to create */
static unsigned major = 0;				/* save major number assigned to driver */
static struct cdev mydev;				/* to be registered with kernel */
static struct class *mydev_class = NULL;		/* device class to register with sysfs */
static struct device **mydev_device;			/* devices registered with sysfs */

#define MYDEV_RING_SIZE 2048				/* accomodate 2K per ring */
static struct OutRingBuffer **mydev_ring;		/* ring buffer handles */
static char *rwbuf = NULL;				/* temporary buffer for moving data
							   between user-space and device */
static unsigned rwbuf_size = 0;				/* temporary buffer sizes */

/* new function to grow unified rwbuf as needed */
static int
chk_rwbuf(size_t nbuf)
{
	if (nbuf > rwbuf_size) {
		/* use a temporary pointer; we simply did rwbuf = krealloc(), we risk losing
		   the current rwbuf if krealloc() fails */
		void *t = krealloc(rwbuf, nbuf, GFP_KERNEL);

		/* if krealloc succeeded, update rwbuf and rwbuf_size */
		if (t != NULL) {
			rwbuf = t;
			rwbuf_size = nbuf;
			//printk("grew rwbuf to size %d\n", rwbuf_size);
		} else if (!rwbuf_size) {
			/* if rwbuf_size is still zero, means we couldn't allocate at all */
			return -ENOMEM;
		}
	}

	return 0;
}

static ssize_t
mydev_read(struct file *filp, char __user *buf, size_t nbuf, loff_t *offs)
{
	unsigned minor = MINOR(filp->f_dentry->d_inode->i_rdev);
	int rc;

	if ((rc = chk_rwbuf(nbuf))) {
		return rc;
	}
	
	/* read data from device into the tmp buffer, since device cannot write directly
	   to user-space memory */
	nbuf = rb_read(mydev_ring[minor], rwbuf, nbuf);

	/* copy the data from the user-supplied buffer to the tmp buffer, since device cannot
	   directly read from user-space memory */
	copy_to_user(buf, rwbuf, nbuf);

	/* adjust file offset */
	*offs += nbuf;

	/* return number of bytes read */
	return nbuf;
}

static ssize_t
mydev_write(struct file *filp, const char __user *buf, size_t nbuf, loff_t *offs)
{
	unsigned minor = MINOR(filp->f_dentry->d_inode->i_rdev);
	int rc;

	if ((rc = chk_rwbuf(nbuf))) {
		return rc;
	}
	
	/* copy the data from the user-supplied buffer to the tmp buffer, since device cannot
	   directly read from user-space memory */
	copy_from_user(rwbuf, buf, nbuf);

	/* write data out to device */
	nbuf = rb_write(mydev_ring[minor], rwbuf, nbuf);

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


module_param(ndev, uint, 0);		/* allow configuration of # devices */
module_param(major, uint, 0);		/* allow manual specification of major # */

static int __init
mymod_init (void)
{
	int rc;
	dev_t devid;
	int i;

	if ((mydev_device = kmalloc(ndev * sizeof *mydev_device, GFP_KERNEL)) == NULL) {
		return -ENOMEM;
	}

	if ((mydev_ring = kmalloc(ndev * sizeof *mydev_ring, GFP_KERNEL)) == NULL) {
		kfree(mydev_device);
		return -ENOMEM;
	}

	/* Handle static allocation of major number (specified as a module parameter) or
	   otherwise dynamically allocate the major number */

	if (major) {
		/* Use the major number specified */
		devid = MKDEV(major, 0);
		rc = register_chrdev_region(devid, ndev, "mydev");
	} else {
		/* Dynamically allocate a range of ndev major/minor numbers */
		rc = alloc_chrdev_region(&devid, 0, ndev, "mydev");

		/* save major number component of devid */
		major = MAJOR(devid);
	}

	if (rc) {
		/* inability to register major number is fatal */
		printk("Trouble registering major number!\n");
		kfree(mydev_device);
		kfree(mydev_ring);
		return rc;
	}

	/* Time to initialize hardware, for this must be done prior to calling
	   cdev_add(), after which time requests can start coming in */

	for (i = 0; i < ndev; i++) {
		rc = rb_init(&mydev_ring[i], MYDEV_RING_SIZE);

		if (rc) {
			printk("rb_init(%d) returned error %d\n", i, rc);
			mydev_ring[i] = NULL;
		}
	}
	
	/* Initalize the struct cdev structure, and set the table of file
	   operation functions (this function cannot fail) */

	cdev_init(&mydev, &mydev_fops);

	/* Register the char device with the kernel. At this point, driver
	   should be prepared to handle requests (particularly if the device
	   nodes were already created) */

	if ((rc = cdev_add(&mydev, devid, ndev))) {
		/* failure at this point is fatal */
		printk("Trouble registering with kernel!\n");
		for (i = 0; i < ndev; i++) {
			if (mydev_ring[i] != NULL)
				rb_destroy(mydev_ring[i]);
		}
		unregister_chrdev_region(devid, ndev);
		kfree(mydev_device);
		kfree(mydev_ring);
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
		/* Register each device with sysfs. Among other things, this
		   will trigger creation of the device nodes in /dev */

		for (i = 0; i < ndev; i++) {
			if (mydev_ring[i] == NULL) {
				/* don't register devices for which there is no
				   ring buffer */
				mydev_device[i] = NULL;
			} else {
				mydev_device[i] = device_create(mydev_class, NULL, MKDEV(major, i), NULL, "mydev%d", i);
				if (IS_ERR(mydev_device[i])) {
					printk("device_create(%d) returned error %ld\n", i, PTR_ERR(mydev_device[i]));
					mydev_device[i] = NULL;
				}
			}
		}
	}

	printk("Hello from mydev!\n");
	return 0;
}  

static void __exit
mymod_exit(void) 
{
	int i;

	/* deregister from kernel */
	cdev_del(&mydev);

	/* release major/minor numbers allocated to the driver */
	unregister_chrdev_region(MKDEV(major, 0), ndev);

	/* Destroy ring buffers only after cdev interfaces is brought down */
	for (i = 0; i < ndev; i++) {
		if (mydev_ring[i] != NULL)
			rb_destroy(mydev_ring[i]);
	}

	if (mydev_class != NULL) {
		/* deregister devices from sysfs */
		for (i = 0; i < ndev; i++) {
			if (mydev_device[i] != NULL)
				device_destroy(mydev_class, MKDEV(major, i));
		}

		class_destroy(mydev_class);
	}

	kfree(mydev_device);
	kfree(mydev_ring);

	if (rwbuf_size)
		kfree(rwbuf);

	printk("Goodbye from mydev.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Character driver skeleton.");
