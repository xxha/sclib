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
	{ 0xb6d47bc4, "kmalloc_caches" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0xb72397d5, "printk" },
	{ 0x54cfe1c7, "class_unregister" },
	{ 0xb4390f9a, "mcount" },
	{ 0x4b4a333, "class_remove_file" },
	{ 0xd5b89c2b, "class_create_file" },
	{ 0x8e2e50ae, "__class_register" },
	{ 0xde4e44c2, "kmem_cache_alloc" },
	{ 0x37a0cba, "kfree" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "20A7B3E7DCE65904AC4FF69");
