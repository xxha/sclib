CC =	gcc -Wall -O2 -I.
RM =	rm -rf

zombie_process:zombie_process.o
	$(CC) zombie_process.o -o zombie_process -lpthread
zombie_process.o:zombie_process.c
#	$(CC) zombie_process.c -c zombie_process.o -lpthread
clean:
	$(RM) *.o
