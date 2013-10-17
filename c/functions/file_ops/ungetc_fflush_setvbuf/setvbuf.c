#include <stdio.h>

int main(void)
{
	FILE *input, *output;
	char bufr[512];

	input = fopen("file.in", "r+b");
	output = fopen("file.out", "w");

	/* set up input stream for minimal disk access,
	using our own character buffer */
	if (setvbuf(input, bufr, _IOFBF, 512) != 0)
		printf("failed to set up buffer for input file\n");
	else
		printf("buffer set up for input file\n");

	/* set up output stream for line buffering using space that
	will be obtained through an indirect call to malloc */
	if (setvbuf(output, NULL, _IOLBF, 132) != 0)
		printf("failed to set up buffer for output file\n");
	else
		printf("buffer set up for output file\n");

	/* perform file I/O here */
	/* close files */
	fclose(input);
	fclose(output);
	return 0;
}


