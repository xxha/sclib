#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* memset() */

int main()
{
        int pipe_fd[2];
        pid_t pid;
        char buf_r[100];
        char* p_wbuf;
        int r_num;

        memset(buf_r, 0, sizeof(buf_r));

        //创建管道
        if(pipe(pipe_fd) < 0){
                printf("pipe create error!\n");
                return -1;
        } else { 
                printf("pipe create succeed!\n");
	}

        //创建子进程
        if((pid = fork()) == 0){  //pid = 0 为子进程
                printf("\n");
                close(pipe_fd[1]); //关闭写管道描述符
                sleep(2); //睡眠2s， 等待父进程写管道

                if((r_num = read(pipe_fd[0], buf_r, 100)) > 0){
                        printf("%d numbers read from the pipe is: %s\n", r_num, buf_r);
                }
                close(pipe_fd[0]); //关闭读管道描述符
                exit(0); //退出进程
        } else if(pid > 0){       //pid > 0 为父进程
                close(pipe_fd[0]);  //关闭读管道描述符
                if((write(pipe_fd[1], "Hello ", 6)) != -1)
                        printf("parent write1 Hello \n");
                if((write(pipe_fd[1], "Pipe!", 5)) != -1)
                        printf("parent write2 Pipe!\n");
                close(pipe_fd[1]); //关闭写管道描述符

                sleep(3);
                waitpid(pid, NULL, 0); //等待子进程退出
                exit(0);
        }
}

