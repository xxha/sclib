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

        //打开管道, 与fifo_read 的文件�??一样，这样才能通信
        fd = open(FIFO_SERVER, O_WRONLY|O_NONBLOCK, 0);

        if(argc == 1){
                printf("Please send something\n");
                exit(-1);
        }
        //拷贝 命令行敲入的字符串 到w_buf[].
        strcpy(w_buf, argv[1]);

        //向管道写入 w_buf 中的数据
        if((nwrite = write(fd, w_buf, 100)) == -1){
                if(errno == EAGAIN)
                        printf("The FIFI has been read yet. Please try later\n");
        } else
                printf("write %s to the FIFO\n", w_buf);
}

