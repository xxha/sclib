#include <usb.h>
#include <stdio.h>
#include <string.h>
//#include <libusb.h>

struct usb_device *usbio_probe()
{
	struct usb_bus *busses, *bus;
	int c, i, a;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();

	for (bus = busses; bus; bus = bus->next) {
		struct usb_device *dev;

		for (dev = bus->devices; dev; dev = dev->next) {
			struct usb_device_descriptor *desc;

			desc = &(dev->descriptor);
			printf("inside Vendor/Product ID: %04x:%04x\n", desc->idVendor, desc->idProduct);

			if ((desc->idVendor == 0xffff) && (desc->idProduct == 0x0001)) {
				return dev;
			}
		}

	}
	return NULL;
}

void usbio_main(struct usb_device *dev)
{
	usb_dev_handle *dev_handle;

	dev_handle = usb_open(dev);
	if (dev_handle == NULL) {
		printf("USB IO open failed.\n");
		return;
	}
	usb_close(dev_handle);

}

int main()
{
	struct usb_device *dev;
	struct usb_device_descriptor *desc;

	dev = usbio_probe();
	desc = &(dev->descriptor);

	if (dev == NULL) {
		printf("USB I/O card not found!\n");
		return -1;
	}

	printf("USB I/O card found!\n");
	printf("outside Vendor/Product ID : %04x:%04x\n", desc->idVendor, desc->idProduct);

	usbio_main(dev);
	return 0;
}
