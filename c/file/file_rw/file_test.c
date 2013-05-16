/* randbin.c -- random access, binary i/o */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARSIZE 100

int main()
{
	char sen1[ARSIZE] = "hello, I am the first sentence.";
	char sen2[ARSIZE] = "sorry, I am the second sentence.";
	char sen3[ARSIZE] = "badly, I am the third sentence.";
	char temp1[ARSIZE];
	char temp2[ARSIZE];
	char temp3[ARSIZE];
	const char * file = "file1";
	FILE *iofile;

	memset(temp1, 0, ARSIZE);
	memset(temp2, 0, ARSIZE);
	memset(temp3, 0, ARSIZE);

	/* first cycle */
	if ((iofile = fopen(file, "w+")) == NULL)
	{
		fprintf(stderr, "Could not open %s for output.\n", file);
		exit(1);
	}
	fwrite(sen1, 1, strlen(sen1), iofile);
	fclose(iofile);

	if ((iofile = fopen(file, "r+")) == NULL)
	{
		fprintf(stderr, "Could not open %s for random access.\n", file);
		exit(1);
	}
	fread(temp1, 1, 100, iofile);
	printf("temp1 = %s\n", temp1);
	fclose(iofile);

        /* second cycle */
        if ((iofile = fopen(file, "w+")) == NULL)
        {
                fprintf(stderr, "Could not open %s for output.\n", file);
                exit(1);
        }
        fwrite(sen2, 1, strlen(sen2), iofile);
        fclose(iofile);

        if ((iofile = fopen(file, "r+")) == NULL)
        {
                fprintf(stderr, "Could not open %s for random access.\n", file);
                exit(1);
        }
        fread(temp2, 1, 100, iofile);
        printf("temp1 = %s\n", temp2);
        fclose(iofile);

        /* third cycle */
        if ((iofile = fopen(file, "w+")) == NULL)
        {
                fprintf(stderr, "Could not open %s for output.\n", file);
                exit(1);
        }
        fwrite(sen3, 1, strlen(sen3), iofile);
        fclose(iofile);

        if ((iofile = fopen(file, "r+")) == NULL)
        {
                fprintf(stderr, "Could not open %s for random access.\n", file);
                exit(1);
        }
        fread(temp3, 1, 100, iofile);
        printf("temp1 = %s\n", temp3);
        fclose(iofile);

	return 0;
}
