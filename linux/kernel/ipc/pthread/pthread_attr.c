#include <stdlib.h>	 
#include <stdio.h>	 
#include <errno.h>	 
#include <pthread.h>	 

static void *pthread_func_1 (void *para);	 
static void *pthread_func_2 (void *para);	 
	
static void *pthread_func_1(void *para)	 
{	 
	int i = 0;	 
		 
	for (; i < 6; i++)	 
	{		
		sleep(1);
		printf ("This is pthread_1.\n");	 
			
		if (i == 2)	 
		{	 
			pthread_exit (0);	 
		}	 
	}	 
	
	return;	 
}	 
	
static void *pthread_func_2(void *para)	 
{	 
	int i = 0;	 
	
	for (; i < 3; i ++)
	{	 
		sleep(1);
		printf ("This is pthread_2.\n");	 
	}	 
	
	return;	 
}
	
int main (int argc, char** argv)	 
{	 
	pthread_t pt_1 = 0;	 
	pthread_t pt_2 = 0;	 
	pthread_attr_t attr = {0};	 
	int ret = 0;	 
	
	/*初始化属性线程属性*/
	pthread_attr_init (&attr);	 
	pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);	 
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);	 
		 
	ret = pthread_create (&pt_1, &attr, pthread_func_1, NULL);	 
	if (ret != 0)	 
	{	 
		perror ("pthread_1_create");	 
	}	 
		 
	ret = pthread_create (&pt_2, NULL, pthread_func_2, NULL);	 
	if (ret != 0)	 
	{	 
		perror ("pthread_2_create");	 
	}	 
	
	pthread_join (pt_2, NULL);	 
	
	return 0;	 
}	 
	
