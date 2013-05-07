/*======================================================================
    A test program to access /dev/second
    This example is to help understand async IO 
    
    The initial developer of the original code is Baohua Song
    <author@linuxdriver.cn>. All Rights Reserved.
======================================================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

/*接收到异步读信号后的动作*/
void input_handler(int signum)
{
  printf("receive a signal from globalfifo,signalnum:%d\n",signum);
}

main()
{
  int fd, oflags;
  fd = open("/dev/globalfifo", O_RDWR, S_IRUSR | S_IWUSR);
  if (fd !=  - 1)
  {
    //启动信号驱动机制
    signal(SIGIO, input_handler); //让input_handler()处理SIGIO信号

    fcntl(fd, F_SETOWN, getpid()); //把本程序进程号添加进fasync_struct链表

    oflags = fcntl(fd, F_GETFL); //获得标志位信息

    fcntl(fd, F_SETFL, oflags | FASYNC);  //F_SETFL 设置开启FASYNC 1 或 关闭FASYNC 0
    while(1)
    {
    	sleep(5000);
    }
  }
  else
  {
    printf("device open failure\n");
  }
}
