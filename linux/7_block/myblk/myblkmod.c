#include <linux/module.h>

static int __init myblkmod_init(void)
{
	printk(KERN_INFO "myblkmod: module loaded\n");
	return 0;
}

static void myblkmod_exit(void)
{
	printk(KERN_INFO "myblkmod: module unloaded\n");
}

module_init(myblkmod_init);
module_exit(myblkmod_exit);

MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example block device");
