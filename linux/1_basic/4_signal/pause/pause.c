#include <unistd.h>
#include <stdio.h>
#include <signal.h>
void sigroutine(int unused)
{
	printf("Catch a signal SIGINT. \n");
}

int main()
{
	printf("Start a process pid = %d.\n", getpid());
	signal(SIGINT, sigroutine);

	printf("Enter pause mode!\n");
	pause();
	printf("Received a signal.\n");
} 
