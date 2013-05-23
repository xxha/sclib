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
#define NUMBER 256

static char *rd_buf;


//从文件中读出数据
static ssize_t attr_read(struct class *c, char *buf)
{
	int i;
	char *var = buf;

	printk(KERN_INFO "%s\n", __func__);

	for(i = 0; i < NUMBER; i++)
		*var++ = rd_buf[i]; 

	return NUMBER;	
}

//buf 用来保存命令行传输的数据，向文件写入数据
static ssize_t attr_write(struct class *c, const char *buf, size_t count)
{
	int i;
	char *var = (char *)buf;

	printk(KERN_INFO "%s\n", __func__);

	for(i = 0; i < NUMBER; i++)
		rd_buf[i] = *var++; 
	
	return NUMBER;	
}

static void attr_release(struct class *class)
{
	printk(KERN_INFO "%s\n", __func__);
}

//attr_example 会在/sys/class/attr_class/ 目录下创建 test1, test2, test3 文件
static struct class_attribute attr_example[] = {  //这是个二维数组
	__ATTR(rd_o, 0444, attr_read, NULL),      //__ATTR 原型 __ATTR(_name,_mode,_show,_store)
	__ATTR(wr_o, 0222, NULL, attr_write),
	__ATTR(rd_wr, 0666, attr_read, attr_write),
	__ATTR_NULL,
};


static struct class kmalloc_class = {
	.name = "attr_class",   //class下目录名
	.owner = THIS_MODULE,
	.class_attrs = attr_example,
	.class_release = attr_release,
};

static ssize_t file1_show(struct class *cls, char *buf)
{
	int i;
	char *var = buf;

	printk(KERN_INFO "%s\n", __func__);

	for(i = 0; i < NUMBER; i++)
		*var++ = rd_buf[i]; 

	return NUMBER;	
}

static struct class_attribute file_attr = __ATTR_RO(file1);


static int hello_init(void)
{
	int err;
	printk(KERN_INFO "Hello World enter\n");

	rd_buf = kzalloc(NUMBER, GFP_KERNEL);
	if(NULL == rd_buf) {
		printk(KERN_INFO "kmalloc rd_buf failed!\n");
		return -1;
	}

        //create "register_class" directory under /sys/class
	err = class_register(&kmalloc_class);
	if(err < 0) {
		printk(KERN_INFO "register class failed!\n");
		kfree(rd_buf);
		return err;
	}

	//create file1 under /sys/class/attr_class
	err = class_create_file(&kmalloc_class, &file_attr);
	if (err < 0) {
		printk(KERN_INFO "class create file failed!\n");
		kfree(rd_buf);
		return err;
	}
		

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_INFO " Hello World exit\n ");

	//remove file1 from /sys/class/attr_class
	class_remove_file(&kmalloc_class, &file_attr);

	//remove "attr_class" directory from /sys/class
	class_unregister(&kmalloc_class);  //class_unregister 会调用 class->class_release 函数

	kfree(rd_buf);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Song Baohua");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");
