//#include "stdafx.h"

#include"stdio.h"
//#include"conio.h"
#include"string.h"
#include"stdlib.h"
int N;
char mima[20]="mm";

//�����鼮�Ľṹ��

struct book
{
    long num;
    char name[20];
    char publish[20];/*������*/
    struct time
    {
        int day;
        int month;
        int year;
    }t;//t
    int jie;/*�������*/
}bk[20];

int mymainmenu();//���˵�����
int mimayanzheng();//������֤����
int adm();//����Ա���溯��
void xinxi();//¼����Ϣ����
void show();//�鿴��Ϣ����
void xiugai();//�޸���Ϣ����
void xiugaimima();//�޸����뺯��
int peo();//�û����溯��
void jieyue();//�����鼮����
void show_all();//�鿴�����鼮��Ϣ
void show_name();//���������鿴�鼮��Ϣ


//������
void main()
{
int x,x2,s;//s�����ж�������֤���0/1
do{
	system("cls");
	x=mymainmenu();//���˵�����***
	switch(x)
	{
	case 1: system("cls"); //�������Աģʽ
			s=mimayanzheng();//������֤���������а���������ʾ***
			do
			{
				if(s==1)
				{	system("cls");
					x2=adm();//����Ա���溯������������ָ��***
					
						switch(x2)
						{
							case 1:system("cls");xinxi();break;//¼����Ϣ����***
							case 2:system("cls");show();break;//�鿴��Ϣ����***
							case 3:system("cls");xiugai();break;//�޸���Ϣ����***
							case 4:system("cls");xiugaimima();break;//�޸����뺯��***
						}
					
				}
				else 
					{
						printf("�����������");	
						printf("�밴��������˳�\n");
						getchar();
						break;}
			}while(x2!=5);
			break;//

	case 2:		do
				{
					system("cls");//�����û�����
					x2=peo();//�û����溯��***
					switch(x2)
					{
					case 1:system("cls");show();break;//�鿴ͼ�麯��***
					case 2:system("cls");jieyue();break;//����ͼ�麯��***
					}
				}while(x2!=3);
				break;
	}
}while(x!=3);
	system("cls");
	printf("thank you for your coming!\n");
	printf("�밴��������˳�\n");
	getchar();

}

int mymainmenu()//���˵�����
{
	int a;
	printf("*****************************************************\n");
	printf("*****************��ӭ����ͼ�����ϵͳ****************\n");
	printf("                 ��ѡ�������                        \n");
	printf("                 1���Թ���Ա��ݽ���\n");
	printf("                 2���Զ�����ݽ���\n");
	printf("                 3���˳�ϵͳ\n");
	scanf("%d",&a);//����scanf("%d\n",&a);
	return a;
}
///*
int mimayanzheng()//������֤����
{
	char b[20];
	printf("*****************���������룺************************\n");
//	gets(b);  //Ϊ����gets(b)ѭ����ͳ���ֻ�ǿ�������������֤�Ľ���??????????????????????????????????
    scanf("%s",b);
	return (!strcmp(mima,b));

}
//*/

int adm()//����Ա���溯��
{
	int a;
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************��ӭ�������Աϵͳ******************\n");
	printf("                 ��ѡ�������                        \n");
	printf("                 1�������鼮��Ϣ\n");
	printf("                 2���鿴�鼮��Ϣ\n");
	printf("                 3���޸��鼮��Ϣ\n");
	printf("                 4���޸�����\n");
	printf("                 5���˳�ϵͳ\n");
	scanf("%d",&a);
	return a;
}

void xinxi()//¼����Ϣ����
{
	int i;
	printf("*****************************************************\n");
	printf("*****************�����鼮��Ϣ************************\n");
	printf("����������鼮������\n");
	scanf("%d",&N);
	for(i=1;i<=N;i++)
	{
		printf("�������%d����ı�ţ�\n",i);
		scanf("%d",&bk[i-1].num);
		printf("�������%d��������֣�\n",i);
		scanf("%s",&bk[i-1].name);
		printf("�������%d����ĳ����磺\n",i);
		scanf("%s",&bk[i-1].publish);
		printf("�������%d����ĳ�����/��/�գ��Կո��������\n",i);
		scanf("%d %d %d",&bk[i-1].t.year,&bk[i-1].t.month,&bk[i-1].t.day);
		printf("�������%d�����Ƿ�ɽ裨1/0����\n",i);
		scanf("%d",&bk[i-1].jie);
		printf("�����鼮�ɹ�\n");
	}
	printf("�밴��������˳�\n");
	getchar();

}

void show_all()  //�鿴�����鼮����
{
	int i;
	for(i=0;i<N;i++)
	{
		printf("��%d�������Ϣ��\n",i+1);
		printf("��ţ�%d\n",bk[i].num);
		printf("������%s\n",bk[i].name);
		printf("�����磺%s\n",bk[i].publish);
		printf("�������ڣ�%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("�Ƿ��ѽ��ģ�%d\n",bk[i-1].jie);
	}
	printf("�밴��������˳�\n");
	getchar();
}

void show_name() //���������鿴�鼮��Ϣ
{
	int i;
	char s[20];
	printf("��������Ҫ���ҵ��鼮���֣�\n");
//	gets(s);//�??�β��У���������������������������������������������
	scanf("%d",s);
	for(i=0;(i<N)&&(!strcmp(s,bk[i].name));i++)
	{
		printf("�����У����Ժ�......");
	}
	if (i<N)
	{
		printf("��Ҫ�????���鼮��Ϣ���£�\n");
		printf("��ţ�%d\n",bk[i].num);
		printf("������%s\n",bk[i].name);
		printf("�����磺%s\n",bk[i].publish);
		printf("�������ڣ�%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("�Ƿ��ѽ��ģ�%d\n",bk[i-1].jie);
	}
	else
		printf("�Բ����������ҵ��鼮������\n");

	printf("�????��������˳�\n");
	getchar();//���壬����������˳�
}


void show()//�鿴��Ϣ����//�������鼮���ֲ鿴��ϢΪ��
{
	int a;
	printf("*****************************************************\n");
	printf("*****************�鿴�鼮��Ϣ************************\n");
	printf("��ѡ�������\n");
	printf("1���鿴�����鼮��Ϣ\n");
	printf("2�����������鿴�鼮��Ϣ\n");
	printf("3��������һ�??�˵�\n");
	scanf("%d",&a);
	switch(a)//ע��۲�δ��ѭ����switch�����У�ѭ����Ϊ�˷�����һ���˵�
	{
	case 1:show_all();break;//�鿴�����鼮����****
	case 2:show_name();break;//�����������????����Ϣ����****
	}
	printf("�밴��������˳�\n");
	getchar();//���壬����������˳�
}


void xiugai()//�޸��鼮��Ϣ
{
	int i;
	char s[20];
	printf("��������Ҫ�޸ĵ��鼮���֣�\n");
//	gets(s);
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("�����У����Ժ�......");
	}
	if (i<N)
	{
		printf("��Ҫ���ҵ��鼮��Ϣ���£�\n");
		printf("��ţ�%d\n",bk[i].num);
		printf("������%s\n",bk[i].name);
		printf("�����磺%s\n",bk[i].publish);
		printf("�������ڣ�%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("�Ƿ��ѽ��ģ�%d\n",bk[i].jie);

		printf("�����������鼮��Ϣ��\n");
		printf("���������ı��??��\n");
		scanf("%d",&bk[i].num);
		printf("���������������??\n",i);
		scanf("%s",&bk[i].name);
		printf("���������ĳ��????��\n",i);
		scanf("%s",&bk[i].publish);
		printf("��������??�ĳ�����/��/�գ��Կո��������\n",i);
		scanf("%d %d %d",&bk[i].t.year,&bk[i].t.month,&bk[i].t.day);
		printf("������������??�ɽ裨1/0����\n",&i);
		scanf("%d",&bk[i].jie);
		printf("���??�鼮��Ϣ�ɹ�\n");

		printf("�޸ĺ���鼮��Ϣ���£�\n");
		printf("��ţ�%d\n",bk[i].num);
		printf("������%s\n",bk[i].name);
		printf("�����磺%s\n",bk[i].publish);
		printf("�������ڣ�%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("�??���ѽ��ģ�%d\n",bk[i].jie);

	}
	else
		printf("�Բ��������??�ҵ��鼮������\n");

	printf("�밴��������˳�\n");
	getchar();//���壬����������˳�

}


void xiugaimima()//�޸�����
{
	char b[20],c[20];
	printf("*****************************************************\n");
	printf("*********************�޸�����************************\n");
	printf("������ԭʼ���룺\n");
	scanf("%s",b);
	if (!strcmp(b,mima))
	{
		printf("�����������룺\n");
		scanf("%s",b);
		printf("���������������룺\n");
		scanf("%s",c);
		if(!strcmp(b,c))
		{
			strcpy(mima,b);
			printf("�����??�óɹ���������Ϊ:%s\n",mima);
		}
		else
			printf("���������??���벻һ�£���������ʧ�ܣ�\n");

	}
	else
		printf("���������??��\n");

	printf("�밴��������˳�\n");
	getchar();
}


int peo()//�û����溯��
{
	int a;
	printf("*****************��ӭ���????��ϵͳ******************\n");
	printf("                 ���??�������                        \n");
	printf("                 1���鿴�鼮��Ϣ\n");
	printf("                 2������ͼ��\n");
	printf("                 3���˳�ϵͳ\n");
	scanf("%d",&a);
	return a;
}


void jieyue()//�����鼮����
{
	int i;
	char s[20];
	printf("��������Ҫ���ĵ��鼮���֣�\n");
//	gets(s);//ͬ���??���³�������ʱ����ֻ��һ�����������޷���������
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("���????�����Ժ�......");
	}
	if (i<N)
		{
			printf("��Ҫ���ĵ����??��Ϣ���£�\n");
			printf("��ţ�%d\n",bk[i].num);
			printf("������%s\n",bk[i].name);
			printf("�����磺%s\n",bk[i].publish);
			printf("�������ڣ�%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
			printf("�Ƿ��ѽ��ģ�%d\n",bk[i-1].jie);

			if(bk[i-1].jie)
				printf("�Բ��𣬸����ѽ����\n");
			else 
				{
					bk[i-1].jie=1;
					printf("���ĳɹ���\n");
				}
		}
	else
		printf("�Բ����������ҵ��鼮�����ڡ�\n");


	printf("�밴��������˳�\n");
	getchar();

}
