/*
 *	device i/o memory mapp
 */

/*xxxxxx-
#include <linux/module.h>

#if defined (CONFIG_SMP)
#define __SMP__
#endif


#if defined(CONFIG_MODVERSIONS)
#define MODVERSIONS
#include <config/modversions.h>
#endif
*/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>

#include <linux/init.h>		/*xxxxxx+*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/config.h>
#include <linux/sched.h>
#include <linux/slab.h>
/*#include <linux/wrapper.h>*/	/*xxxxxx-*/
#include <linux/kdev_t.h>
#include <asm/uaccess.h>

#include <asm/io.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>

#include <linux/fs.h>
#include <linux/mm.h>

#include <linux/devfs_fs_kernel.h>
#include <linux/smp_lock.h>
#include <linux/device.h>

#include "iomap.h"
#define DEBUG_IOMAP 0

Iomap *iomap_dev[IOMAP_MAX_DEVS];

MODULE_DESCRIPTION("devmmap, device mapping i/o memory");
MODULE_LICENSE("GPL");


int iomap_remove(Iomap *idev)
{
	iounmap(idev->ptr);
	MSG("buffer at 0x%lx removed\n", idev->base);
	return 0;
}

int iomap_setup(Iomap *idev)
{
	/* remap the i/o region */
	idev->ptr = ioremap(idev->base, idev->size);
	
	MSG("setup: 0x%lx extending 0x%lx bytes\n", idev->base, idev->size);
	return 0;
}

static int iomap_mmap(struct file *file, struct vm_area_struct *vma)
{
	Iomap *idev = iomap_dev[MINOR(file->f_dentry->d_inode->i_rdev)];
	unsigned long size;
	
	/* no such device */
	if (!idev->base)
		return -ENXIO;
	
	/* size must be a multiple of PAGE_SIZE */
	size = vma->vm_end - vma->vm_start;
	if (size % PAGE_SIZE)
		return -EINVAL;

	/*
	 * Accessing memory above the top the kernel knows about or
	 * through a file pointer that was marked O_SYNC will be
	 * done non-cached.
	 */
	if (file->f_flags & O_SYNC){
		/*vma->vm_page_prot.pgprot |= (_PAGE_NO_CACHE | _PAGE_GUARDED);*/
		vma->vm_page_prot |= (_PAGE_NO_CACHE | _PAGE_GUARDED);
	}

	/* Don't try to swap out physical pages.. */
	vma->vm_flags |= VM_RESERVED;

	/*
	 * Dump addresses that are real memory to a core file.
	 */
	if (idev->base < __pa(high_memory) && !(file->f_flags & O_SYNC))
		vma->vm_flags &= ~VM_IO;


	//if (remap_page_range(vma->vm_start, idev->base, size, vma->vm_page_prot))  
	if (remap_pfn_range(vma, vma->vm_start, idev->base>>PAGE_SHIFT, size, vma->vm_page_prot))
		return -EAGAIN;

	MSG("region mmapped\n");
	return 0;
}

static int iomap_ioctl(struct inode *inode, struct file *file,
		       unsigned int cmd, unsigned long arg)
{
	Iomap *idev = iomap_dev[MINOR(inode->i_rdev)];
	
	switch (cmd) {
		/* create the wanted device */
		case IOMAP_SET: {
			/* if base is set, device is in use */
			if (idev->base)
				return -EBUSY;
			if (copy_from_user(idev, (Iomap *)arg, sizeof(Iomap)))
				return -EFAULT;
			/* base and size must be page aligned */
			if (idev->base % PAGE_SIZE || idev->size % PAGE_SIZE) {
				idev->base = 0;
				return -EINVAL;
			}
			MSG("setting up minor %d\n", MINOR(inode->i_rdev));
			iomap_setup(idev);
			return 0;
		}

		case IOMAP_GET: {
			/* maybe device is not set up */
			if (!idev->base)
				return -ENXIO;
			if (copy_to_user((Iomap *)arg, idev, sizeof(Iomap)))
				return -EFAULT;
			return 0;
		}

		case IOMAP_CLEAR: {
			long tmp;
			/* if base is set, device is in use */
			if (!idev->base)
				return -EBUSY;
			if (get_user(tmp, (long *)arg))
				return -EFAULT;
			memset_io(idev->ptr, tmp, idev->size);
			return 0;
		}
		
	}

	return -ENOTTY;
}

static ssize_t iomap_read(struct file *file, char *buf, size_t count,
			  loff_t *offset)
{
	Iomap *idev = iomap_dev[MINOR(file->f_dentry->d_inode->i_rdev)];
	char *tmp;	
	
	/* device not set up */
	if (!idev->base)
		return -ENXIO;

	/* beyond or at end? */
	if (file->f_pos >= idev->size)
		return 0;

	tmp = (char *) kmalloc(count, GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	/* adjust access beyond end */
	if (file->f_pos + count > idev->size)
		count = idev->size - file->f_pos;

	/* get the data from the mapped region */
	if(idev->busBitSize == 8)
	{
	    int i;

	    for (i = 0; i < count; i++){
		tmp[i] = readb(idev->ptr+file->f_pos+i);
	    }
	}
	else if(idev->busBitSize == 16)
	{ 
	    int i;
	    u16 *wd = (u16*)tmp;
	    count /= 2;
	    for (i = 0; i < count; i++)
	    {
	    	wd[i] = readw(idev->ptr+file->f_pos+i*2);
	    }
	    count *= 2;
	}
	else
 	    memcpy_fromio(tmp, idev->ptr+file->f_pos, count);

	/* copy retrieved data back to app */
	if (copy_to_user(buf, tmp, count)) 
	{
		kfree(tmp);
		return -EFAULT;
	}

	file->f_pos += count;
	kfree(tmp);
	return count;
}

static ssize_t iomap_write(struct file *file, const char *buf, size_t count,
			   loff_t *offset)
{
	Iomap *idev = iomap_dev[MINOR(file->f_dentry->d_inode->i_rdev)];
	char *tmp;
	
	/* device not set up */
	if (!idev->base)
		return -ENXIO;

	/* end of mapping? */
	if (file->f_pos >= idev->size)
		return 0;

	tmp = (char *) kmalloc(count, GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	/* adjust access beyond end */
	if (file->f_pos + count > idev->size)
		count = idev->size - file->f_pos;

	/* get user data */
	if (copy_from_user(tmp, buf, count)) 
	{
		kfree(tmp);
		return -EFAULT;
	}


	/* write data to i/o region */
	if (idev->busBitSize == 8)
	{
		int i;
		for (i = 0; i < count; i++){
			writeb(tmp[i], idev->ptr + file->f_pos + i);
		}
	}
	else if (idev->busBitSize == 16)
	{
		int i;
		u16 *wd = (u16*)tmp;
		count /= 2;
		for (i = 0; i < count; i++)
		    writew(wd[i], idev->ptr + file->f_pos + i * 2);	  
	}
	else	
		memcpy_toio(idev->ptr+file->f_pos, tmp, count);

	file->f_pos += count * (idev->busBitSize / 8);
	/*
	if (idev->busBitSize == 16)
		file->f_pos += count * 2;
	else if (idev->busBitSize == 8) 
		file->f_pos += count;
	*/

	kfree(tmp);
	return count;
}

static loff_t iomap_llseek(struct file *file, loff_t offset, int origin)
{
	long long retval;
	Iomap *idev = iomap_dev[MINOR(file->f_dentry->d_inode->i_rdev)];

	/* device not set up */
	if (!idev->base){
#if DEBUG_IOMAP
		printk("lseek: base = 0\n");
#endif
		return -ENXIO;
	}
	
	switch (origin) {
		case 2: offset += idev->size;
			break;
		case 1:
			offset += file->f_pos;
			break;
	}
	retval = -EINVAL;
	if(offset >= 0) {
	 	if(idev->busBitSize == 16){
			if(offset % 2){
#if DEBUG_IOMAP
				  printk("lseek: offset%2\n");
#endif
				  return retval;
			}
		}
		if (/*offset != file->f_pos &&*/ offset < idev->size) {
			file->f_pos = offset;
			retval = offset;
		}
	}
#if DEBUG_IOMAP
	printk("lseek: retval = %d\n", retval);
#endif
	return retval;
}

int iomap_open(struct inode *inode, struct file *file)
{
	int minor = MINOR(inode->i_rdev);
	
	/* no such device */
	if (minor >= IOMAP_MAX_DEVS)
		return -ENXIO;

	//MOD_INC_USE_COUNT;
	return 0;
}

int iomap_release(struct inode *inode, struct file *file)
{
	//MOD_DEC_USE_COUNT;
	return 0;
}

struct file_operations iomap_fops = {
        .owner		= THIS_MODULE,
	.llseek		= iomap_llseek,
	.read		= iomap_read,
	.write		= iomap_write,
	.ioctl		= iomap_ioctl,
	.mmap		= iomap_mmap,
	.open		= iomap_open,
	.release	= iomap_release,
};

static int __init devmmap_init(void)
{
	int res, i;
	
	/* register device with kernel */
	res = register_chrdev(IOMAP_MAJOR, "iomap", &iomap_fops);
	if (res) {
		MSG("can't register device with kernel\n");
		return res;
	}
	
	for (i = 0; i < IOMAP_MAX_DEVS; i++) {
		iomap_dev[i] = (Iomap *) kmalloc(sizeof(Iomap), GFP_KERNEL);
		iomap_dev[i]->base = 0;
	}
	
	MSG("module loaded\n");
	return 0;
}

static void __exit devmmap_exit(void)
{
	int i = 0;
	Iomap *tmp;
	
	/* delete the devices */
	for (tmp = iomap_dev[i]; i < IOMAP_MAX_DEVS; tmp = iomap_dev[++i]) {
		if (tmp->base)
			iomap_remove(tmp);
		kfree(tmp);
	}

	unregister_chrdev(IOMAP_MAJOR, "iomap");
	MSG("unloaded\n");
	return;
}

module_init(devmmap_init);
module_exit(devmmap_exit);	
