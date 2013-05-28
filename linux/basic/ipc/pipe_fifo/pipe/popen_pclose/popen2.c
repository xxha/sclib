#include <stdio.h>
#include <string.h>
#include <strings.h>

int main(){
        FILE *file_pipe;
        unsigned char string[32];
        int i,j,pos,len;
	unsigned int partition_num;

        memset(string,0x00,sizeof(string));

        /* get the number of partition */
        file_pipe=popen("ls -l /dev/sda? |grep \"sda\" -c","r");
        if(NULL==file_pipe){
                printf("Failed to retrive the partition information.\n");
                return -1;
        }
        fgets(string,sizeof(string),file_pipe);

        pclose(file_pipe);
        printf("string = %s\n",string);
        memset(string,0x00,sizeof(string));

	partition_num=atoi(string);
        printf("partition_num = 0x%x\n", partition_num);
        /*detect the detail partiton infomation.*/
        sprintf(string,"/dev/sda%d",partition_num-2);
        printf("string2 = %s\n",string);

        return 0;
}

