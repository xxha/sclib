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

MODULE_LICENSE("Dual BSD/GPL");

static struct class kmalloc_class = {
	.name = "register_class",
	.owner = THIS_MODULE,
};

static int hello_init(void)
{
	int err;
	printk(KERN_INFO "Hello World enter\n");

        //create "register_class" directory under /sys/class
	err = class_register(&kmalloc_class);
	if(err < 0) {
		printk(KERN_INFO "register class failed!\n");
		return err;
	}

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_INFO " Hello World exit\n ");
	//remove "register_class" directory from /sys/class
	class_unregister(&kmalloc_class);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Song Baohua");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");
