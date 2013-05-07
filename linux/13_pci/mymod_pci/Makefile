obj-m := mydev.o
mydev-objs := mymod.o

# With this simplified Makefile, type 'make' to build your module
# Type 'make install' to build and install the module to the target file system

KERNELDIR=$(shell ls -d ~/linux-2.6.[0-9]*)
TARGETDIR=/targetfs

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules modules_install INSTALL_MOD_PATH=$(TARGETDIR)
