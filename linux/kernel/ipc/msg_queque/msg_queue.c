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

        // /tmp/2 就是一个消息队列
        key = ftok("/tmp/2", 'a'); //‘a’ 啥意义， 
        //该函数得到该消息队列的键值key

        printf("key = [0x%x]\n", key);
        msgid = msgget(key, IPC_CREAT|0666); //根据key，得到该消息队列描述符。

        if(msgid == -1){
                printf("create error\n");
                return -1;
        }

        msgbuf.mtype = getpid(); //制定消息类型
        strcpy(msgbuf.data, "test haha");

        //发送消息队列，不阻塞方式
        ret = msgsnd(msgid, &msgbuf, sizeof(msgbuf.data), IPC_NOWAIT);
        if(ret == -1){
                printf("send message error\n");
                return -1;
        }

        //接收消息队列
        memset(&msgbuf, 0, sizeof(msgbuf));
        ret = msgrcv(msgid, &msgbuf, sizeof(msgbuf.data), getpid(), IPC_NOWAIT);
        if(ret == -1){
                printf("receive message error\n");
                return -1;
        }

        printf("receive msg = [%s]\n", msgbuf.data);
}
