/*
 * @Brief driver for veex cpld keypad port
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/kd.h>
#include <linux/fs.h>
#include <linux/kd.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/input.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/fsl_devices.h>
#include <linux/slab.h>
#include <asm/mach/keypad.h>



/* Keypad module name */
#define MOD_NAME "cpldkpd"

/* Maximum number of keys */
#define MAX_ROW 	4
#define MAX_COL 	3
#define CPLD_MAXKE 	(MAX_ROW*MAX_COL)

#define CPLDKPD_ROW 	4
#define CPLDKPD_COL 	3


/*
 * This define indicates break scancode for every key release.
 * A constant of 128 is added to the key press scancode.
 */
#define CPLD_KEYRELEASE 	128

#define CPLD_KPD_INT  60

#define DEBUG 1
#ifdef DEBUG  
#define dprintk(x...)    printk("----------:" x )  
#else  
#define dprintk(x...)  
#endif  


/*
 * This enum represents the keypad state machine to
 * maintain debounce logic for key press/release
 */
enum KeyState {
	/* Key press state */
	KStateUp,

	/* Key press debounce state*/
	KStateFirstDown,

	/* Key release state */
	KStateDown,

	/* Key release debounce state */
	KStateFirstUp
};

/* Keypad private data structure */
struct keypad_priv {

	/* Keypad state machine */
	enum KeyState iKeyState;

	/* Number of rows configured in keypad matrix */
	unsigned long kpd_rows;

	/* Number of columns configured in keypad matrix */
	unsigned long kpd_cols;

	/* Timer used for keypad polling */
	struct timer_list poll_timer;

	/* The base address */
	void __iomem *base;
};

/* This structure holds the keypad private data structure */
static struct keypad_priv kpd_dev;

/* Indicates if the keypad device is enabled */
static unsigned int key_pad_enabled;

/* Input device structure */
static struct input_dev *cpldkpd_dev;

/* This static variable indicates whether a key event is pressed/released */
static unsigned short KPress;

/* cur_rcmap and prev_rcmap is used to detect key press and release */
static unsigned short *cur_rcmap;       /* max 64 bits (8x8 matrix) */
static unsigned short *prev_rcmap;

/* Debounce polling period(10ms) in system ticks. */
static unsigned short KScanRate = (10 * HZ) / 1000;

static struct keypad_data *keypad;

static int has_leaning_key;

/* These arrays are used to store press and release scancodes. */
static short **press_scancode;
static short **release_scancode;

static const unsigned short *cpldkpd_keycodes;
static unsigned short cpldkpd_keycodes_size;

static irqreturn_t cpld_kpd_interrupt(int irq, void *dev_id)
{
	unsigned short reg_val;

	del_timer(&kpd_dev.poll_timer);

	//cpld_kpd_handle_timer(0);

	return IRQ_RETVAL(1);
}

static void cpld_kpd_free_allocated(void)
{
	int i;

	if (press_scancode) {
		for(i = 0; i < kpd_dev.kpd_rows; i++) {
			if(press_scancode[i])
				kfree(press_scancode[i]);
		}
		kfree(press_scancode);
	}

	if (release_scancode) {
		for (i = 0; i < kpd_dev.kpd_rows; i++) {
			if(release_scancode[i])
				kfree(release_scancode[i]);
		}
		kfree(release_scancode);
	}

	if (cur_rcmap)
		kfree(cur_rcmap);

	if (prev_rcmap)
		kfree(prev_rcmap);

//	if (cpldkpd_dev)
//		input_free_device(cpldkpd_dev);
}

static int cpld_kpd_open(struct input_dev *dev)
{
	return 0;
}

static void cpld_kpd_close(struct input_dev *dev)
{
}


#ifdef CONFIG_PM
static int cpld_kpd_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int cpld_kpd_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define cpld_kpd_suspend NULL
#define cpld_kpd_resume NULL
#endif

static u16 keymapping[] = {
	KEY_F1, KEY_HOME, KEY_DOWN,
	KEY_F2, KEY_UP, KEY_ENTER,
	KEY_F3, KEY_LEFT, KEY_ESC,
	KEY_F4, KEY_RIGHT, 0
};

static struct keypad_data cpldkpd_data = {
	.rowmax = 4,
	.colmax = 3,
	.irq = CPLD_KPD_INT,
	.learning = 0,
	.delay = 2,
	.matrix = keymapping,

};

static int cpld_kpd_probe(struct platform_device *pdev)
{
	int i, irq;
	int retval;
	unsigned int reg_val;
	struct resource *res;

	dprintk("%s.\n", __func__);
	kpd_dev.kpd_cols = cpldkpd_data.colmax;
	kpd_dev.kpd_rows = cpldkpd_data.rowmax;
	key_pad_enabled = 0;

/*  need to map cpld keypad address.
/*	kpd_dev.base = ioremap();
	if(!kpd_dev.base)
		return -ENOMEM;
*/

/*  need to get cpld keypad irq*/
 	irq = cpldkpd_data.irq;

	has_leaning_key = cpldkpd_data.learning;
	cpldkpd_keycodes = &cpldkpd_data.matrix; 
	cpldkpd_keycodes_size = CPLDKPD_ROW * CPLDKPD_COL;

	cpldkpd_dev = input_allocate_device();
	if(!cpldkpd_dev) {
		printk(KERN_ERR
			"cpldkpd_dev: not enough memory for input device\n");
		retval = -ENOMEM;
		goto err1;
	}

	cpldkpd_dev->keycode = (void *)cpldkpd_keycodes;
	cpldkpd_dev->keycodesize = sizeof(cpldkpd_keycodes[0]);
	cpldkpd_dev->keycodemax = cpldkpd_keycodes_size;
	cpldkpd_dev->name = "cpldkpd";
	cpldkpd_dev->id.bustype = BUS_HOST;
	cpldkpd_dev->open = cpld_kpd_open;
	cpldkpd_dev->close = cpld_kpd_close;

	retval = input_register_device(cpldkpd_dev);
	if(retval < 0) {
		printk(KERN_ERR
			"cpldkpd_dev: failed to register input device.\n");
		goto err2;
	}

	/* allocate required memory */
	press_scancode = kmalloc(kpd_dev.kpd_rows * sizeof(press_scancode[0]),
					GFP_KERNEL);
	release_scancode = kmalloc(kpd_dev.kpd_rows * sizeof(release_scancode[0]),
					GFP_KERNEL);
	if(!press_scancode || !release_scancode) {
		retval = -ENOMEM;
		goto err3;
	}

	for(i = 0; i < kpd_dev.kpd_rows; i++) {
		press_scancode[i] = kmalloc(kpd_dev.kpd_cols 
				* sizeof(press_scancode[0][0]), GFP_KERNEL);
		release_scancode[i] = kmalloc(kpd_dev.kpd_cols
				* sizeof(release_scancode[0][0]), GFP_KERNEL);
		if (!press_scancode[i] || !release_scancode[i]) {
			retval = -ENOMEM;
			goto err3;
		}
	}

	cur_rcmap = kmalloc(kpd_dev.kpd_rows * sizeof(cur_rcmap[0]), 
						GFP_KERNEL);
	prev_rcmap = kmalloc(kpd_dev.kpd_rows * sizeof(prev_rcmap[0]), 
						GFP_KERNEL);
	if(!cur_rcmap || !prev_rcmap) {
		retval = -ENOMEM;
		goto err3;
	}

	__set_bit(EV_KEY, cpldkpd_dev->evbit);

	for (i = 0; i < cpldkpd_keycodes_size; i++)
		__set_bit(cpldkpd_keycodes[i], cpldkpd_dev->keybit);
	for (i = 0; i < kpd_dev.kpd_rows; i++) {
		memset(press_scancode[i], -1,
			sizeof(press_scancode[0][0]) * kpd_dev.kpd_cols);
		memset(release_scancode[i], -1,
			sizeof(release_scancode[0][0]) * kpd_dev.kpd_cols);
	}
	memset(cur_rcmap, 0, kpd_dev.kpd_rows * sizeof(cur_rcmap[0]));
	memset(prev_rcmap, 0, kpd_dev.kpd_rows * sizeof(prev_rcmap[0]));

	key_pad_enabled = 1;
	/* Initialize the polling timer */
	init_timer(&kpd_dev.poll_timer);

	retval = request_irq(irq, cpld_kpd_interrupt, 0, MOD_NAME, MOD_NAME);
	if(retval) {
		pr_debug("CPLDKPD: request_irq(%d) error %d\n",
				irq, retval);
		goto err3;
	}

	device_init_wakeup(&pdev->dev, 1);

	return 0;

err3:
	cpld_kpd_free_allocated();
err2:
	input_free_device(cpldkpd_dev);
err1:
	free_irq(irq, MOD_NAME);
	return retval;
}

static int cpld_kpd_remove(struct platform_device *pdev)
{
	unsigned short reg_val;
	KPress = 0;

	del_timer(&kpd_dev.poll_timer);
	free_irq(cpldkpd_data.irq, MOD_NAME);

	input_unregister_device(cpldkpd_dev);
	cpld_kpd_free_allocated();

	return 0;
}


/* This structure contains pointers to the power management callback functions. */
static struct platform_driver cpld_kpd_driver = {
	.driver = {
			.name = "cpld_keypad",
			.bus = &platform_bus_type,
		  },
	.suspend = cpld_kpd_suspend,
	.resume = cpld_kpd_resume,
	.probe = cpld_kpd_probe,
	.remove = cpld_kpd_remove
};

static int __init cpld_kpd_init(void)
{
	printk(KERN_INFO "CPLD keypad loaded\n");
	platform_driver_register(&cpld_kpd_driver);

	return 0;
}

static void __exit cpld_kpd_cleanup(void)
{
	platform_driver_unregister(&cpld_kpd_driver);
}

module_init(cpld_kpd_init);
module_exit(cpld_kpd_cleanup);







































