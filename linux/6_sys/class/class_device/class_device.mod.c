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
	{ 0x355992de, "module_layout" },
	{ 0x5e43d4a1, "cdev_del" },
	{ 0xd86b4bb5, "cdev_init" },
	{ 0xf53ed33d, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xb72397d5, "printk" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0x374a690c, "device_create" },
	{ 0x6e18b7ea, "cdev_add" },
	{ 0xb8857caf, "class_destroy" },
	{ 0x20e01bc9, "__class_create" },
	{ 0xd6c963c, "copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F66B515F355C7985EAA31D8");
