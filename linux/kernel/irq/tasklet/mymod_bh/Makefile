obj-m := mydev.o
mydev-objs := mymod_bh.o process_irq.o

# With this simplified Makefile, type 'make' to build your module
# Type 'make install' to build and install the module to the target file system

KERNELDIR=/lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules modules_install INSTALL_MOD_PATH=$(TARGETDIR)

clean:
	rm -rf *.o *.order *.symvers *.mod.c *.ko 
