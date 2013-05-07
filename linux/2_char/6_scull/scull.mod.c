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
	{ 0x94a6467e, "kobject_put" },
	{ 0x5e43d4a1, "cdev_del" },
	{ 0x126bda94, "per_cpu__current_task" },
	{ 0xb6d47bc4, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xd86b4bb5, "cdev_init" },
	{ 0xb279da12, "pv_lock_ops" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x8eb58c6e, "__wake_up_sync" },
	{ 0x1017a642, "no_llseek" },
	{ 0x973873ab, "_spin_lock" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x45d11c43, "down_interruptible" },
	{ 0x1f305993, "kobject_set_name" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xff964b25, "param_set_int" },
	{ 0xe754e2ef, "tty_devnum" },
	{ 0x50e82dcf, "nonseekable_open" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0xb72397d5, "printk" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x30c1ee24, "fasync_helper" },
	{ 0xb4390f9a, "mcount" },
	{ 0x748caf40, "down" },
	{ 0x6e18b7ea, "cdev_add" },
	{ 0x7dceceac, "capable" },
	{ 0xde4e44c2, "kmem_cache_alloc" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0x4292364c, "schedule" },
	{ 0x642e54ac, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0x3f1899f1, "up" },
	{ 0x9b3f9338, "kill_fasync" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0xd6c963c, "copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "01D52C7AFCBB13B1CFDEE76");
