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
  
        //����1k�Ĺ����ڴ棬�ɶ���д  
        if((shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1){  
                fprintf(stderr, "Create share memory error: %s\n\a",  
                                                        strerror(errno));  
        }  
  
        //�����ӽ���  
        if(fork()){ //�����̣�д����  
                p_addr = shmat(shmid, 0 , 0); //ӳ�䵽��������  
                                             //��ַΪ0��˵����ϵͳ�Զ�ָ����ַ
                memset(p_addr, '\0', 1024);  
                strncpy(p_addr, argv[1], 1024);  //���������������ַ��������ڴ�  
                wait(NULL);  
                exit(0);  
        } else{  
                sleep(1);  //�ӽ��̣�������  
                c_addr = shmat(shmid, 0 ,0); //�����ڴ�ӳ�䵽�ӽ���  
                printf("Client get %s\n", c_addr);  
		shmdt(p_addr);        //���ӳ�� 
                exit(0);  
        }

}  
