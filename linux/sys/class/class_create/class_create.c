/*======================================================================
    A simple kernel module: "hello world"
         
    The initial developer of the original code is Baohua Song
    <author@linuxdriver.cn>. All Rights Reserved.
======================================================================*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/device.h> //这个头文件包含class_create()

MODULE_LICENSE("Dual BSD/GPL");


static struct class *simple_class = NULL;

static int hello_init(void)
{
	printk(KERN_INFO "Hello World enter\n");

        //create "test_class" directory under /sys/class
	simple_class = class_create(THIS_MODULE, "test_class");
	if (IS_ERR(simple_class)) {
                printk("class_create() returned error %ld\n", PTR_ERR(simple_class));
                simple_class = NULL;
	}

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_INFO " Hello World exit\n ");

	//remove "test_class" under /sys/class
        if (simple_class != NULL) {
                class_destroy(simple_class);
        }

}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Song Baohua");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");
