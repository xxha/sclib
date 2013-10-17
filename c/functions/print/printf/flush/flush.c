#include <string.h>
#include <stdio.h>
//#include <.h>
//#include <io.h>

void flush(FILE *stream)
{
	int duphandle;
	/* flush the stream's internal buffer */
	fflush(stream);
	/* make a duplicate file handle */
	duphandle = dup(fileno(stream));
	/* close the duplicate handle to flush the DOS buffer */
	close(duphandle);
}

int main(void)
{
	FILE *stream;
	char msg[] = "This is a test";
	/* create a file */
	stream = fopen("DUMMY.FIL", "w");
	/* write some data to the file */
	fwrite(msg, strlen(msg), 1, stream);
	system("clear");
	printf("Press any key to flush DUMMY.FIL:");
	getchar();

	/* flush the data to DUMMY.FIL without\
	closing it */
	flush(stream);
	printf("\nFile was flushed, Press any key to quit:");
	getchar();
	return 0;
}
