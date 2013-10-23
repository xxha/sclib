CC =	gcc -Wall -O2 -I.
RM = 	rm *.o

all: spec

spec: spec.o
	$(CC) spec.o -o spec -lrt
spec.o: spec.c

clean:
	$(RM) *.o
