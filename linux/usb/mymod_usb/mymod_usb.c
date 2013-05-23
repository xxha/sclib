#include <linux/module.h>
#include <linux/usb.h>

static struct usb_device_id mydev_usb_devices[] = {
	{ .idVendor = 0xffff, .idProduct = 0xffff },
	{ 0 }
};

MODULE_DEVICE_TABLE(usb, mydev_usb_devices);

static int __devinit
mydev_probe(struct usb_interface *intf, const struct usb_device_id *ent)
{
	struct usb_device *udev = interface_to_usbdev(intf);
	printk("mydev probe() called for device %04x %04x\n", udev->descriptor.idVendor, udev->descriptor.idProduct);
	return -ENODEV;
}

static void __devexit
mydev_disconnect(struct usb_interface *intf)
{
	printk("mydev got removed!\n");
}

static struct usb_driver mydev_driver = {
	.name = "mydev",
	.id_table = mydev_usb_devices,
	.probe = mydev_probe,
	.disconnect = mydev_disconnect
};

static int __init
mymod_init (void)
{
	printk("Hello from my module.\n"); 
	return usb_register(&mydev_driver);
}

static void __exit
mymod_exit(void) 
{
	usb_deregister(&mydev_driver);
	printk("Goodbye from my module.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Basic hello world-type module.");
