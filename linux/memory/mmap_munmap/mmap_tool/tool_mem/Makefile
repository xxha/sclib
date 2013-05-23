# Misc Compiling Stuff
CROSS_COMPILE = powerpc-860-linux-gnu-

#compile tools
AS	  =	$(CROSS_COMPILE)as
CC	  =	$(CROSS_COMPILE)gcc
LD	  =	$(CROSS_COMPILE)ld
AR	  =	$(CROSS_COMPILE)ar
STRIP =	$(CROSS_COMPILE)strip

CFLAGS = -O2 -pipe -fsigned-char

objects = memtool.o

memtool : $(objects)
	$(CC) -o $@ $^

$(objects) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

dep : %.c
	@echo "[DEP] $@ ----- $<"
	@$(CC) -M $< > $@
	
.PHONY : clean

clean:
	rm -f *.o
	rm startup
	
	
