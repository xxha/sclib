#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

static struct kobject *parent;
static struct kobject *child;
static struct kset *c_kset;

static unsigned long flag = 1;

static ssize_t att_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	size_t count = 0;
	count += sprintf(&buf[count], "%lu\n", flag);

	return count;
}

static ssize_t att_store(struct kobject *kobj, struct attribute *attr, 
				const char *buf, size_t count)
{
	flag = buf[0] - '\0';

	switch(flag) {
	case 0:
		kobject_uevent(kobj, KOBJ_ADD);
		break;
	case 1:
		kobject_uevent(kobj, KOBJ_REMOVE);
		break;
	case 2:
		kobject_uevent(kobj, KOBJ_CHANGE);
		break;
	case 3:
		kobject_uevent(kobj, KOBJ_MOVE);
		break;
	case 4:
		kobject_uevent(kobj, KOBJ_ONLINE);
		break;
	case 5:
		kobject_uevent(kobj, KOBJ_OFFLINE);
		break;
	default:
		break;
	}
	return count;

}

static struct attribute cld_att = {
	.name = "child_attr",
	.mode = S_IRUGO | S_IWUSR,
};

static const struct sysfs_ops att_ops = {
	.show = att_show,
	.store = att_store,
};

static struct kobj_type cld_ktype = {
	.sysfs_ops = &att_ops,  //sysfs_ops of cld_ktype is att_ops
};

static int kobj_demo_init(void)
{
	int err;

	printk(KERN_INFO "%s\n", __func__);

	//create pa_obj directory under /sys
	parent = kobject_create_and_add("parent_kobj", NULL);

	//allocate child memory
	child = kzalloc(sizeof(*child), GFP_KERNEL);
	if(!child)
		return PTR_ERR(child);

	//create child_kset directory under /sys/pa_obj
	c_kset = kset_create_and_add("child_kset", NULL, parent);
	if(!c_kset)
		return -1;

	child->kset = c_kset;

	//create child_kobj directory under /sys/parent_kobj
	err = kobject_init_and_add(child, &cld_ktype, parent, "child_kobj");
	if(err)
		return err;

	//create "cldatt" named file under /sys/pa_obj/cld_obj 
	err = sysfs_create_file(child, &cld_att);

	return err;
}

static void kobj_demo_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);

	//remove /sys/pa_obj/cld_obj/cldatt file
	sysfs_remove_file(child, &cld_att);

	//remove /sys/pa_obj/c_kset directory
	kset_unregister(c_kset);

	//remove /sys/pa_obj/cld_obj directory
	kobject_del(child);

	//remove /sys/pa_obj directory
	kobject_del(parent);
}

module_init(kobj_demo_init);
module_exit(kobj_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lamdoc@jlu.com");
MODULE_DESCRIPTION("A simple kernel module to demo the kobject behavior");


























