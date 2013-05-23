UTILS_VERSION=1.0.1
CC=arm-none-linux-gnueabi-gcc
#CC=gcc
CFLAG=
UTIS_AUTHOR=$(shell whoami)
UTIS_HOST_OS=$(shell uname)
UTIS_TIME=$(shell date)
uuu:main.c mtd-self.h
	@rm -f version.h
	@echo "#ifndef __VERSION_H ">>version.h
	@echo "#define __VERSION_H ">>version.h
	@echo "#define UTILS_VERSION " "\""$(UTILS_VERSION)"\"" >>version.h
	@echo "#define UTILS_HOST_OS" "\""$(UTIS_HOST_OS)"\"" >>version.h
	@echo "#define UTILS_AUTHOR" "\""$(UTIS_AUTHOR)"\"">>version.h
	@echo "#define UTILS_TIME " "\""$(UTIS_TIME)"\"" >>version.h
	@echo "#endif"  >>version.h
	$(CC) $(CFLAG) main.c -o uuu
	cp uuu /var/lib/tftpboot
clean:
	rm -fr *.o uuu
