#include <stdio.h>
#define N 4
#include <string.h>
#include <malloc.h>



void main(void)
{
    char *p[N];
	char c,d;
	int i=0;    
void maxstring(char **p);
void replaceString(char **p,char c,char d);
    printf("plese enter %d strings:\n",N);
    for(i=0;i<N;i++)
	{
		p[i]=(char*)malloc(100*sizeof(char));
		//scanf("%s", p[i]);  //为何不行？用scanf则最后一个子程序不能得到正确结果，无法改变字符串数组内容
		gets(p[i]);
	}

    maxstring(p);//获取最大的字符串	

	printf("please enter the char to be replaced by the second char: \n");
	scanf("%c %c",&c,&d);//中间留个空格，这样输入时不容易出错。
	replaceString(p,c,d);
}

void maxstring(char **p)
{    
	int ii;
	char *pp=(char*)malloc(sizeof *pp);
    	   
    strcpy(pp,*p);
   
	for(ii=1;ii<N;ii++)                                       
        if(strcmp(pp,p[ii])<0)
                strcpy(pp,p[ii]);


	printf("the maximum string is:\n");
	puts(pp);
}

void replaceString(char **p,char c,char d)  
{
	char *q=(char*)malloc(sizeof *p);
	int i;

 	for(i=0;i<N;i++)
        {
                for(q=*(p+i);*q!='\0';q++)
                {        
                        if(*q==c)
                               *q = d;                        }
                puts(*(p+i));
        }


}

