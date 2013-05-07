#include <string.h>
#include <stdio.h>

int main(void)
{
	FILE *stream;
	char string[] = "This is a test";
	fpos_t filepos;

	/* open a file for update */
	stream = fopen("DUMMY.FIL", "w+");

	/* write a string into the file */
	fwrite(string, strlen(string), 1, stream);

	/* report the file pointer position */
	fgetpos(stream, &filepos);
	printf("The file pointer is at byte\
		%ld\n", filepos);

	fclose(stream);
	return 0;
}
