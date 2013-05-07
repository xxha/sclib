#include <linux/module.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include "echodev.h"

#define MYDEV_MAX_NDEV 16		/* maximum number of devices supported */

static unsigned ndev = 1;		/* number of devices to create */
static unsigned major = 0;		/* save major number assigned to driver */

module_param(ndev, uint, 0);		/* allow configuration of # devices */
module_param(major, uint, 0);		/* allow manual specification of major # */

struct mydev_struct {
	struct echodev *e;
	int open_count;
};

struct mydev_struct *mydev_table[MYDEV_MAX_NDEV];

static void
mydev_rx(void *priv, const unsigned char *data, int len)
{
	struct tty_struct *tty = priv;
	int rc;

	printk("received %d bytes from device\n", len);
}

static int
mydev_open(struct tty_struct * tty, struct file * filp)
{
	struct mydev_struct *dev = mydev_table[tty->index];

	if (dev == NULL) {
		if (!(dev = kmalloc(sizeof *dev, GFP_KERNEL)))
			return -ENOMEM;

		if (!(dev->e = echodev_init(tty, mydev_rx))) {
			kfree(dev);
			return -ENOMEM;
		}

		mydev_table[tty->index] = dev;
	}

	++dev->open_count;
	tty->driver_data = dev;

	return 0;
}

static void
mydev_close(struct tty_struct *tty, struct file * filp)
{
	struct mydev_struct *dev = tty->driver_data;

	if (dev) {
		--dev->open_count;
		if (dev->open_count <= 0) {
			echodev_destroy(dev->e);
			kfree(dev);
			mydev_table[tty->index] = NULL;
		}
	}
}

static int
mydev_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct mydev_struct *dev = tty->driver_data;
	printk("attempting to send %d bytes to device\n", count);
	return echodev_write(dev->e, buf, count);
}

static int
mydev_writeroom(struct tty_struct *tty)
{
	struct mydev_struct *dev = tty->driver_data;
	int room = echodev_free(dev->e);
	printk("mydev_writeroom returns %d\n", room);
	return room;
}

static struct tty_operations mydev_ops = {
	.open = mydev_open,
	.close = mydev_close,
	.write = mydev_write,
	.write_room = mydev_writeroom,
};

static struct tty_driver *mydev_driver;

static int __init
mymod_init (void)
{
	int rc;

	/* Ensure ndev isn't too big */

	if (ndev > MYDEV_MAX_NDEV) {
		printk("Capping ndev to %d\n", MYDEV_MAX_NDEV);
		ndev = MYDEV_MAX_NDEV;
	}

	if (!(mydev_driver = alloc_tty_driver(ndev)))
		return -ENOMEM;

	mydev_driver->owner = THIS_MODULE;
	mydev_driver->driver_name = "mydev_tty";
	mydev_driver->name = "mydevtty";
	mydev_driver->major = major;
	mydev_driver->type = TTY_DRIVER_TYPE_SERIAL;
	mydev_driver->subtype = SERIAL_TYPE_NORMAL;
	mydev_driver->flags = TTY_DRIVER_REAL_RAW;
	mydev_driver->init_termios = tty_std_termios;
	mydev_driver->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	tty_set_operations(mydev_driver, &mydev_ops);

	if ((rc = tty_register_driver(mydev_driver))) {
		printk("tty_register_driver() returned %d\n", rc);
		put_tty_driver(mydev_driver);
		return rc;
	}

	/* Initialize hardware */

	
	printk("Hello from mydev!\n");
	return 0;
}  

static void __exit
mymod_exit(void) 
{
	tty_unregister_driver(mydev_driver);
	put_tty_driver(mydev_driver);

	printk("Goodbye from mydev.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Character driver skeleton.");
