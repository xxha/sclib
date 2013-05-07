#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIFO_SERVER "/tmp/myfifo"

main(int argc, char** argv)
{
        int fd;
        char w_buf[100];
        int nwrite;

        //�򿪹ܵ�, ��fifo_read ���ļ��??һ������������ͨ��
        fd = open(FIFO_SERVER, O_WRONLY|O_NONBLOCK, 0);

        if(argc == 1){
                printf("Please send something\n");
                exit(-1);
        }
        //���� ������������ַ��� ��w_buf[].
        strcpy(w_buf, argv[1]);

        //��ܵ�д�� w_buf �е�����
        if((nwrite = write(fd, w_buf, 100)) == -1){
                if(errno == EAGAIN)
                        printf("The FIFI has been read yet. Please try later\n");
        } else
                printf("write %s to the FIFO\n", w_buf);
}

