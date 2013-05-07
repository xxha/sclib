#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void my_func(int sign_no)
{
        if(sign_no == SIGINT)
                printf("I have get SIGINT!\n");
        else if(sign_no == SIGQUIT)
                printf("I have get SIGQUIT!\n");
}

int main()
{
        printf("Waiting for signal SIGINT or SIGQUIT\n");

        //ע���źŴ�����
        signal(SIGINT, my_func);
        signal(SIGQUIT, my_func);

        pause(); //�ȴ��źţ��յ��źŲŻ�����ȴ�
        exit(0);
}

