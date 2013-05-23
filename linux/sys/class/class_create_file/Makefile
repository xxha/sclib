obj-m := class_create_file.o
KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -f *.o *.ko *.mod.* modules.* Module.* 
