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
	{ 0xbe6a2279, "cdev_del" },
	{ 0x4f0b454f, "cdev_init" },
	{ 0x1f74dda0, "mem_map" },
	{ 0x6a9f26c9, "init_timer_key" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xb2aabe2, "__alloc_pages_nodemask" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe83fea1, "del_timer_sync" },
	{ 0xb72397d5, "printk" },
	{ 0x7e547717, "kunmap" },
	{ 0xb4390f9a, "mcount" },
	{ 0x46085e4f, "add_timer" },
	{ 0xa3fba4b4, "contig_page_data" },
	{ 0x6087741d, "cdev_add" },
	{ 0x453771f0, "__free_pages" },
	{ 0x6f83f30, "kmap" },
	{ 0xaac64d33, "remap_pfn_range" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "E45C341A68C53D252044DA4");
