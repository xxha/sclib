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
	{ 0xfb69281a, "kset_create_and_add" },
	{ 0xb6d47bc4, "kmalloc_caches" },
	{ 0x63ac7c68, "kobject_uevent" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0xf94e938b, "kobject_del" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x94de3391, "kobject_create_and_add" },
	{ 0xb72397d5, "printk" },
	{ 0x8eae69ba, "kobject_init_and_add" },
	{ 0xb4390f9a, "mcount" },
	{ 0xde4e44c2, "kmem_cache_alloc" },
	{ 0x5a3e14b5, "sysfs_create_file" },
	{ 0x1471ea2, "kset_unregister" },
	{ 0x71cbf0b9, "sysfs_remove_file" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4288433A73BD1CF345986B0");
