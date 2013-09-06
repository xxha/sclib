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
	{ 0x7d09bae2, "module_layout" },
	{ 0x1fc45350, "cdev_alloc" },
	{ 0xdb6c1991, "device_remove_file" },
	{ 0xbe6a2279, "cdev_del" },
	{ 0x4f0b454f, "cdev_init" },
	{ 0xcf20bd97, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb72397d5, "printk" },
	{ 0xfb43ad1b, "fasync_helper" },
	{ 0xb4390f9a, "mcount" },
	{ 0xfda5d96e, "device_create" },
	{ 0x3e3f2d09, "device_create_file" },
	{ 0x6087741d, "cdev_add" },
	{ 0xa8f3ff3d, "class_destroy" },
	{ 0x41865c90, "kill_fasync" },
	{ 0xdecb46af, "__class_create" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7D897E42EB1E7B3F8049C04");
