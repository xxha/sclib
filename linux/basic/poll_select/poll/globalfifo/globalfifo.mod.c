#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xee584c90, "module_layout" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x37a0cba, "kfree" },
	{ 0x88473f72, "cdev_del" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x68dfc59f, "__init_waitqueue_head" },
	{ 0xe798e8ea, "cdev_add" },
	{ 0xb0bc3bba, "cdev_init" },
	{ 0x7807eace, "kmem_cache_alloc_trace" },
	{ 0x440a4045, "kmalloc_caches" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x2e60bace, "memcpy" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x4292364c, "schedule" },
	{ 0x3a013b7d, "remove_wait_queue" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xd7bd3af2, "add_wait_queue" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x215f9a25, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0xc4554217, "up" },
	{ 0xdd1a2871, "down" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "38476D0B5AB62D27DBC28E4");
