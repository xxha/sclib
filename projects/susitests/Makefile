#Makefile for Linux SUSI 3.02

LIB_NAME = libSUSI-3.02
LIB_LINK_NAME = $(LIB_NAME).so
LINKLIB = -Wl,-rpath,./ $(LIB_LINK_NAME)
CFLAGS = -Wall -Werror -O2
DEMO_PROG = demo_brightness demo_cpu demo_gpio demo_hwm demo_iic demo_smbus demo_vga demo_watchdog test_gpio

% : %.c
	$(CC) $(CFLAGS) -o $(basename $< .c) $< $(LINKLIB) 


default: $(DEMO_PROG)

install:
	rm  -f /usr/lib/$(LIB_NAME).*
	cp -a $(LIB_NAME).* /usr/lib/

uninstall:
	rm  /usr/lib/$(LIB_NAME).*

clean:
	rm -f $(DEMO_PROG)

