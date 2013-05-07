#include <linux/module.h>
#include <linux/blkdev.h>

static int myblkmod_major;
static spinlock_t myblkmod_lock;
static struct request_queue *myblkmod_queue;
static struct gendisk *myblkmod_disk;

static void myblkmod_request(struct request_queue *queue)
{
}

static struct block_device_operations myblkmod_ops = {
	.owner = THIS_MODULE
};

static int __init myblkmod_init(void)
{
	/* Register device */
	myblkmod_major = register_blkdev(0, "myblkdev");
	if (myblkmod_major <= 0) {
		printk(KERN_INFO "myblkmod: registration failed\n");
		return -EBUSY;
	}

	/* Create request queue */
	spin_lock_init(&myblkmod_lock);
	myblkmod_queue = blk_init_queue(myblkmod_request, &myblkmod_lock);
	if (myblkmod_queue == NULL) {
		printk(KERN_INFO "myblkmod: request queue init failed\n");
		unregister_blkdev(myblkmod_major, "myblkdev");
		return -ENOMEM;
	}
	blk_queue_logical_block_size(myblkmod_queue, 512);

	/* Allocate memory for disk info */
	myblkmod_disk = alloc_disk(16);
	if (myblkmod_disk == NULL) {
		printk(KERN_INFO "myblkmod: disk struct alloc failed\n");
		blk_cleanup_queue(myblkmod_queue);
		unregister_blkdev(myblkmod_major, "myblkdev");
		return -ENOMEM;
	}

	/* Initialise disk info */
	myblkmod_disk->major = myblkmod_major;
	myblkmod_disk->first_minor = 0;
	myblkmod_disk->fops = &myblkmod_ops;
	myblkmod_disk->queue = myblkmod_queue;
	snprintf(myblkmod_disk->disk_name, 32, "myblkdeva");
	set_capacity(myblkmod_disk, 2 * 1024 * 16);		/* 16MB */

	/* Make disk available */
	printk(KERN_INFO "myblkmod: module loaded\n");
	add_disk(myblkmod_disk);

	return 0;
}

static void myblkmod_exit(void)
{
	del_gendisk(myblkmod_disk);
	blk_cleanup_queue(myblkmod_queue);
	unregister_blkdev(myblkmod_major, "myblkdev");

	printk(KERN_INFO "myblkmod: module unloaded\n");
}

module_init(myblkmod_init);
module_exit(myblkmod_exit);

MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example block device");
