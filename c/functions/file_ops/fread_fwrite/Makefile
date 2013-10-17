CC =	gcc -Wall -O2 -I.
RM =	rm -rf

all: append randbin1

append: append.o
append.o: append.c

randbin1: randbin1.o
randbin1.o: randbin1.c

randbin2: randbin2.o
randbin2.o: randbin2.c

clean:
	$(RM)  *.o
