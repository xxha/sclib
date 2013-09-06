obj-m := hello.o
KERNELDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

default:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers 
