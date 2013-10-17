CC = gcc -Wall -O2 -I.
RM = rm -rf 

all: rm_test system fprintf_nl

rm_test: rm_test.o
rm_test.o: rm_test.c

system: system.o
system.o: system.c

fprintf_nl: fprintf_nl.o
fprintf_nl.o: fprintf_nl.c

clean:
	$(RM) *.o 
