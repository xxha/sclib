#include <stdlib.h> 
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
  
#define PERM S_IRUSR | S_IWUSR  
  
int main(int argc, char **argv)  
{  
        int shmid;  
        char *p_addr, *c_addr;  
  
        if(argc != 2){  
                fprintf(stderr,"Usage: %s\n\a", argv[0]);  
                exit(0);  
        }  
  
        //创建1k的共享内存，可读可写  
        if((shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1){  
                fprintf(stderr, "Create share memory error: %s\n\a",  
                                                        strerror(errno));  
        }  
  
        //创建子进程  
        if(fork()){ //父进程，写操作  
                p_addr = shmat(shmid, 0 , 0); //映射到父进程中  
                                             //地址为0，说明让系统自动指定地址
                memset(p_addr, '\0', 1024);  
                strncpy(p_addr, argv[1], 1024);  //拷贝命令行输入字符到共享内存  
                wait(NULL);  
                exit(0);  
        } else{  
                sleep(1);  //子进程，读操作  
                c_addr = shmat(shmid, 0 ,0); //共享内存映射到子进程  
                printf("Client get %s\n", c_addr);  
		shmdt(p_addr);        //解除映射 
                exit(0);  
        }

}  
