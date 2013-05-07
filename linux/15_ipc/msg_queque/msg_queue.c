#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct msg_buf{
        int mtype;
        char data[255];
};

int main()
{
        key_t key;
        int msgid;
        int ret;
        struct msg_buf msgbuf;

        // /tmp/2 ����һ����Ϣ����
        key = ftok("/tmp/2", 'a'); //��a�� ɶ���壬 
        //�ú����õ�����Ϣ���еļ�ֵkey

        printf("key = [0x%x]\n", key);
        msgid = msgget(key, IPC_CREAT|0666); //����key���õ�����Ϣ������������

        if(msgid == -1){
                printf("create error\n");
                return -1;
        }

        msgbuf.mtype = getpid(); //�ƶ���Ϣ����
        strcpy(msgbuf.data, "test haha");

        //������Ϣ���У���������ʽ
        ret = msgsnd(msgid, &msgbuf, sizeof(msgbuf.data), IPC_NOWAIT);
        if(ret == -1){
                printf("send message error\n");
                return -1;
        }

        //������Ϣ����
        memset(&msgbuf, 0, sizeof(msgbuf));
        ret = msgrcv(msgid, &msgbuf, sizeof(msgbuf.data), getpid(), IPC_NOWAIT);
        if(ret == -1){
                printf("receive message error\n");
                return -1;
        }

        printf("receive msg = [%s]\n", msgbuf.data);
}
