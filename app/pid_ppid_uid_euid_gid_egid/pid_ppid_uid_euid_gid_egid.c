#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
	pid_t pid;
	pid = getpid();

	pid_t ppid;
	ppid = getppid();

	fprintf(stdout, "pid = %d, ppid = %d\n", pid, ppid);

	uid_t uid, euid;
	gid_t gid, egid;

	uid = getuid();
	euid = geteuid();
	gid = getgid();
	egid = getegid();

	fprintf(stdout, "uid = %d, euid = %d\n", uid, euid);
	fprintf(stdout, "gid = %d, egid = %d\n", gid, egid);

	sleep(500);
	return 0;
}
