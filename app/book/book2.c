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
		//scanf("%s", p[i]);  //Ϊ�β��У���scanf�����һ���ӳ����ܵõ���ȷ������޷��ı��ַ�����������
		gets(p[i]);
	}

    maxstring(p);//��ȡ�����ַ���	

	printf("please enter the char to be replaced by the second char: \n");
	scanf("%c %c",&c,&d);//�м������ո���������ʱ�����׳���
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

