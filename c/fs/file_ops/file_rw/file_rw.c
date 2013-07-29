#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fdr,fdw;
	char filepath[20] = "1file";
	char sen1[100] = "hello, I am the first sentence.";
	char sen2[100] = "sorry, I am the second sentence.";
	char sen3[100] = "badly, I am the third sentence.";
	char temp1[100];
	char temp2[100];
	char temp3[100];

	memset(temp1, 0, 100);
	memset(temp2, 0, 100);
	memset(temp3, 0, 100);

	/* first cycle */
	if((fdw = open(filepath, O_WRONLY)) == -1) {
		printf("file not found,open failed!\n");
		return -1;
	} else {
		int w_size = write(fdw,sen1,strlen(sen1));
		close(fdw);
		printf("w_size = %d\n", w_size);
	}

	if((fdr = open(filepath, O_RDONLY)) == -1) {
		printf("open failed!\n");
		return -1;
	} else {
		int r_size = read(fdr, temp1, 100);
		close(fdr);
		printf("r_size = %d\n", r_size);
		printf("temp1 = %s\n",temp1);
	}

	/* second cycle */
        if((fdw = open(filepath, O_WRONLY)) == -1) {
                printf("file not found,open failed!\n");
                return -1;
        } else {
                int w_size = write(fdw,sen2,strlen(sen2));
                close(fdw);
                printf("w_size = %d\n", w_size);
        }

        if((fdr = open(filepath, O_RDONLY)) == -1) {
                printf("open failed!\n");
                return -1;
        } else {
                int r_size = read(fdr, temp2, 100);
                close(fdr);
                printf("r_size = %d\n", r_size);
                printf("temp2 = %s\n",temp2);
        }

	/* third cycle*/
        if((fdw = open(filepath, O_WRONLY)) == -1) {
                printf("file not found,open failed!\n");
                return -1;
        } else {
                int w_size = write(fdw,sen3,strlen(sen3));
                close(fdw);
                printf("w_size = %d\n", w_size);
        }

        if((fdr = open(filepath, O_RDONLY)) == -1) {
                printf("open failed!\n");
                return -1;
        } else {
                int r_size = read(fdr, temp3, 100);
                close(fdr);
                printf("r_size = %d\n", r_size);
                printf("temp3 = %s\n",temp3);
        }


	return 0;
}
