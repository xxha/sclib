#include <stdlib.h>
#include <stdio.h>

void showpos(FILE *stream);

int main(void)
{
	FILE *stream;
	fpos_t filepos;

	/* open a file for update */
	stream = fopen("DUMMY.FIL", "w+");

	/* save the file pointer position */
	fgetpos(stream, &filepos);

	/* write some data to the file */
	fprintf(stream, "This is a test");

	/* show the current file position */
	showpos(stream);

	/* set a new file position, display it */
	if (fsetpos(stream, &filepos) == 0)
		showpos(stream);
	else
	{
		fprintf(stderr, "Error setting file \
			pointer.\n");
		exit(1);
	}

	/* close the file */
	fclose(stream);
	return 0;
}

void showpos(FILE *stream)
{
	fpos_t pos;

	/* display the current file pointer
	position of a stream */
	fgetpos(stream, &pos);
	printf("File position: %ld\n", pos);
}
