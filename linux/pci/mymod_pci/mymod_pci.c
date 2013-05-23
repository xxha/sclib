#include <linux/module.h>
#include <linux/pci.h>

static DEFINE_PCI_DEVICE_TABLE(mydev_pci_devices) = {
	{ PCI_DEVICE(PCI_ANY_ID,PCI_ANY_ID) },
	{ 0 }
};

MODULE_DEVICE_TABLE(pci, mydev_pci_devices);

static int __devinit
mydev_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	printk("mydev probe() called for device %04x %04x\n", pdev->vendor, pdev->device);
	return -ENODEV;
}

static void __devexit
mydev_remove(struct pci_dev *pdev)
{
	printk("mydev got removed!\n");
}

static struct pci_driver mydev_driver = {
	.name = "mydev",
	.id_table = mydev_pci_devices,
	.probe = mydev_probe,
	.remove = mydev_remove
};

static int __init
mymod_init (void)
{
	printk("Hello from my module.\n"); 
	return pci_register_driver(&mydev_driver);
}

static void __exit
mymod_exit(void) 
{
	pci_unregister_driver(&mydev_driver);
	printk("Goodbye from my module.\n"); 
}  

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_AUTHOR("Me"); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Basic hello world-type module.");
