CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: fflush setvbuf ungetc

fflush: fflush.o
fflush.o: fflush.c

setvbuf: setvbuf.o
setvbuf.o: setvbuf.c

ungetc: ungetc.o
ungetc.o: ungetc.c

clean:
	$(RM)  *.o
