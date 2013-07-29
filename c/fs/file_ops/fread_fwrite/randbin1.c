/* randbin.c -- random access, binary i/o */
#include <stdio.h>
#include <stdlib.h>
#define ARSIZE 5000

int main()
{
    char numbers[ARSIZE] = "if you forget me, i want you to know one thing,you know how this is, if i look at the crystal moon";
    char value;
    const char * file = "/usr/file1";
    int i;
    long pos;
    FILE *iofile;

    /* create a set of double values */

    /* attempt to open file */
    if ((iofile = fopen(file, "w+")) == NULL)
    {
        fprintf(stderr, "Could not open %s for output.\n", file);
        exit(1);
    }

    /* write array in binary format to file */
    fwrite(numbers, sizeof (double), ARSIZE, iofile);

    fclose(iofile);
#if 0
    if ((iofile = fopen(file, "rb")) == NULL)
    {
        fprintf(stderr,
            "Could not open %s for random access.\n", file);
        exit(1);
    }

    /* read selected items from file */
    printf("Enter an index in the range 0-%d.\n", ARSIZE - 1);
    scanf("%d", &i);
    while (i >= 0 && i < ARSIZE)
    {
        pos = (long) i * sizeof(double); /* calculate offset */
        fseek(iofile, pos, SEEK_SET);    /* go there */
        fread(&value, sizeof (double), 1, iofile);
        printf("The value there is %f.\n", value);
        printf("Next index (out of range to quit):\n");
        scanf("%d", &i);
    }
    /* finish up */
    fclose(iofile);
    puts("Bye!");
#endif
    return 0;
}
