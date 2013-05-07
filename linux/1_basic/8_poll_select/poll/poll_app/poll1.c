/*linux poll”√∑®*/
#include <sys/poll.h>
#include <stdio.h>
#include <unistd.h> /* STDIN_FILENO */

#define TIMEOUT 1000

int main(void)
{
        struct pollfd fds[2];
        int ret;
        int i = 0;

        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;
       // fds[1].fd = STDOUT_FILENO;
       // fds[1].events = POLLOUT;

        while(i<5){
	        ret = poll(fds,1,TIMEOUT);
                if(ret == -1){
                        perror("poll");
                        return 1;
                }

                if(!ret){
                        printf("%d seconds elapsed. \n",TIMEOUT);
                }

                if(fds[0].revents & POLLIN){
                        printf("stdin is readable\n");
                }

         //       if(fds[1].revents & POLLOUT){\
                        printf("stdout is writeable\n");
         //       }

                i++;
        }
        return 0;
}
