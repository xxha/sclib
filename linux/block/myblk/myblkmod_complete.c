#include <linux/module.h>
#include <linux/blkdev.h>

static int myblkmod_major;
static spinlock_t myblkmod_lock;
static struct request_queue *myblkmod_queue;
static struct gendisk *myblkmod_disk;
static char *myblkmod_data;

static void myblkmod_request(struct request_queue *queue)
{
	struct request *req = NULL;
	unsigned long offset, nbytes;

	/* Loop through all blocks in request */
	while (1) {
		/* Get next request */
		if (req == NULL)
			req = blk_fetch_request(queue);

		/* Queue may have been empty */
		if (req == NULL)
			return;

		/* We only support read/write (filesystem) requests */
		if (!blk_fs_request(req)) {
			__blk_end_request_all(req, -EIO);
			continue;
		}

		/* Get address and size of data (from buffer) to read/write */
		/* Remember that the kernel uses 512-byte sectors */
		offset = blk_rq_pos(req) * 512;
		nbytes = blk_rq_cur_sectors(req) * 512;

		/* Read/write data */
		if (rq_data_dir(req)) {
			/* Write... */
			memcpy((myblkmod_data+offset), req->buffer, nbytes);
		} else {
			/* Read... */
			memcpy(req->buffer, (myblkmod_data+offset), nbytes);
		}

		/* All done? */
		if (!__blk_end_request_cur(req, 0))
			req = NULL;
	}
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

	/* Allocate memory for the data */
	myblkmod_data = vmalloc(512 * 2 * 1024 * 16);		/* 16MB */
	if (myblkmod_data == NULL) {
		printk(KERN_INFO "myblkmod: disk buffer alloc failed\n");
		blk_cleanup_queue(myblkmod_queue);
		unregister_blkdev(myblkmod_major, "myblkdev");
		return -ENOMEM;
	}
	memset(myblkmod_data, 0, 512 * 2 * 1024 * 16);

	/* Allocate memory for disk info */
	myblkmod_disk = alloc_disk(16);
	if (myblkmod_disk == NULL) {
		printk(KERN_INFO "myblkmod: disk struct alloc failed\n");
		vfree(myblkmod_data);
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
	vfree(myblkmod_data);
	blk_cleanup_queue(myblkmod_queue);
	unregister_blkdev(myblkmod_major, "myblkdev");

	printk(KERN_INFO "myblkmod: module unloaded\n");
}

module_init(myblkmod_init);
module_exit(myblkmod_exit);

MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example block device");
