#include <linux/interrupt.h>
#include <linux/module.h>

static unsigned long irq_count[16];
module_param_array(irq_count, ulong, NULL, 0444);

extern void process_irq(void);

static void
mymod_interrupt_bh(unsigned long data)
{
	process_irq();
}

static DECLARE_TASKLET(mymod_tasklet, mymod_interrupt_bh, (unsigned long)THIS_MODULE);

static irqreturn_t
mymod_interrupt(int irq, void *dev_id)
{
	++irq_count[irq];
	tasklet_schedule(&mymod_tasklet);
	return IRQ_NONE;
}

static int rc[16];

static int __init
mymod_init (void)
{
	int i;

	for (i = 0; i < 16; i++) {
		irq_count[i] = 0;
		if ((rc[i] = request_irq(i, mymod_interrupt, IRQF_SHARED,"mymod", THIS_MODULE)))
			printk("request_irq(%d) returned %d\n", i, rc[i]);
	}

	return 0;
}  

static void __exit
mymod_exit(void) 
{
	int i;

	for (i = 0; i < 16; i++) {
		if (!rc[i])
			free_irq(i, THIS_MODULE);
	}

	printk("Goodbye from my module.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Basic hello world-type module.");
