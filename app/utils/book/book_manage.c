//#include "stdafx.h"

#include"stdio.h"
//#include"conio.h"
#include"string.h"
#include"stdlib.h"
int N;
char mima[20]="mm";

//定义书籍的结构体

struct book
{
    long num;
    char name[20];
    char publish[20];/*出版社*/
    struct time
    {
        int day;
        int month;
        int year;
    }t;//t
    int jie;/*借阅与否*/
}bk[20];

int mymainmenu();//主菜单函数
int mimayanzheng();//密码验证函数
int adm();//管理员界面函数
void xinxi();//录入信息函数
void show();//查看信息函数
void xiugai();//修改信息函数
void xiugaimima();//修改密码函数
int peo();//用户界面函数
void jieyue();//借阅书籍函数
void show_all();//查看所有书籍信息
void show_name();//依据书名查看书籍信息


//主函数
void main()
{
int x,x2,s;//s用来判断密码验证结果0/1
do{
	system("cls");
	x=mymainmenu();//主菜单函数***
	switch(x)
	{
	case 1: system("cls"); //进入管理员模式
			s=mimayanzheng();//密码验证函数，其中包含界面显示***
			do
			{
				if(s==1)
				{	system("cls");
					x2=adm();//管理员界面函数，包含接收指令***
					
						switch(x2)
						{
							case 1:system("cls");xinxi();break;//录入信息函数***
							case 2:system("cls");show();break;//查看信息函数***
							case 3:system("cls");xiugai();break;//修改信息函数***
							case 4:system("cls");xiugaimima();break;//修改密码函数***
						}
					
				}
				else 
					{
						printf("密码输入错误！");	
						printf("请按任意键后退出\n");
						getchar();
						break;}
			}while(x2!=5);
			break;//

	case 2:		do
				{
					system("cls");//进入用户界面
					x2=peo();//用户界面函数***
					switch(x2)
					{
					case 1:system("cls");show();break;//查看图书函数***
					case 2:system("cls");jieyue();break;//借阅图书函数***
					}
				}while(x2!=3);
				break;
	}
}while(x!=3);
	system("cls");
	printf("thank you for your coming!\n");
	printf("请按任意键后退出\n");
	getchar();

}

int mymainmenu()//主菜单函数
{
	int a;
	printf("*****************************************************\n");
	printf("*****************欢迎进入图书管理系统****************\n");
	printf("                 请选择操作：                        \n");
	printf("                 1、以管理员身份进入\n");
	printf("                 2、以读者身份进入\n");
	printf("                 3、退出系统\n");
	scanf("%d",&a);//错误：scanf("%d\n",&a);
	return a;
}
///*
int mimayanzheng()//密码验证函数
{
	char b[20];
	printf("*****************请输入密码：************************\n");
//	gets(b);  //为何用gets(b)循环体就出错？只是快速闪过密码验证的界面??????????????????????????????????
    scanf("%s",b);
	return (!strcmp(mima,b));

}
//*/

int adm()//管理员界面函数
{
	int a;
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************欢迎进入管理员系统******************\n");
	printf("                 请选择操作：                        \n");
	printf("                 1、插入书籍信息\n");
	printf("                 2、查看书籍信息\n");
	printf("                 3、修改书籍信息\n");
	printf("                 4、修改密码\n");
	printf("                 5、退出系统\n");
	scanf("%d",&a);
	return a;
}

void xinxi()//录入信息函数
{
	int i;
	printf("*****************************************************\n");
	printf("*****************插入书籍信息************************\n");
	printf("请输入插入书籍总数：\n");
	scanf("%d",&N);
	for(i=1;i<=N;i++)
	{
		printf("请输入第%d本书的编号：\n",i);
		scanf("%d",&bk[i-1].num);
		printf("请输入第%d本书的名字：\n",i);
		scanf("%s",&bk[i-1].name);
		printf("请输入第%d本书的出版社：\n",i);
		scanf("%s",&bk[i-1].publish);
		printf("请输入第%d本书的出版年/月/日（以空格隔开）：\n",i);
		scanf("%d %d %d",&bk[i-1].t.year,&bk[i-1].t.month,&bk[i-1].t.day);
		printf("请输入第%d本书是否可借（1/0）：\n",i);
		scanf("%d",&bk[i-1].jie);
		printf("插入书籍成功\n");
	}
	printf("请按任意键后退出\n");
	getchar();

}

void show_all()  //查看所有书籍函数
{
	int i;
	for(i=0;i<N;i++)
	{
		printf("第%d本书的信息：\n",i+1);
		printf("编号：%d\n",bk[i].num);
		printf("书名：%s\n",bk[i].name);
		printf("出版社：%s\n",bk[i].publish);
		printf("出版日期：%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("是否已借阅：%d\n",bk[i-1].jie);
	}
	printf("请按任意键后退出\n");
	getchar();
}

void show_name() //依据书名查看书籍信息
{
	int i;
	char s[20];
	printf("请输入所要查找的书籍名字：\n");
//	gets(s);//�??何不行？？？？？？？？？？？？？？？？？？？？？？？
	scanf("%d",s);
	for(i=0;(i<N)&&(!strcmp(s,bk[i].name));i++)
	{
		printf("查找中，请稍候......");
	}
	if (i<N)
	{
		printf("您要鏌????的书籍信息如下：\n");
		printf("编号：%d\n",bk[i].num);
		printf("书名：%s\n",bk[i].name);
		printf("出版社：%s\n",bk[i].publish);
		printf("出版日期：%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("是否已借阅：%d\n",bk[i-1].jie);
	}
	else
		printf("对不起，您所查找的书籍不存在\n");

	printf("璇????任意键后退出\n");
	getchar();//缓冲，任意键后再退出
}


void show()//查看信息函数//以依据书籍名字查看信息为例
{
	int a;
	printf("*****************************************************\n");
	printf("*****************查看书籍信息************************\n");
	printf("请选择操作：\n");
	printf("1、查看所有书籍信息\n");
	printf("2、依据书名查看书籍信息\n");
	printf("3、返回上一�??菜单\n");
	scanf("%d",&a);
	switch(a)//注意观察未加循环的switch的运行，循环是为了返回上一级菜单
	{
	case 1:show_all();break;//查看所有书籍函数****
	case 2:show_name();break;//依据书名查鐪????籍信息函数****
	}
	printf("请按任意键后退出\n");
	getchar();//缓冲，任意键后再退出
}


void xiugai()//修改书籍信息
{
	int i;
	char s[20];
	printf("请输入所要修改的书籍名字：\n");
//	gets(s);
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("查找中，请稍候......");
	}
	if (i<N)
	{
		printf("您要查找的书籍信息如下：\n");
		printf("编号：%d\n",bk[i].num);
		printf("书名：%s\n",bk[i].name);
		printf("出版社：%s\n",bk[i].publish);
		printf("出版日期：%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("是否已借阅：%d\n",bk[i].jie);

		printf("请重新输入书籍信息：\n");
		printf("请输入该书的编�??：\n");
		scanf("%d",&bk[i].num);
		printf("请输入该书的名字�??\n",i);
		scanf("%s",&bk[i].name);
		printf("请输入该书的出鐗????：\n",i);
		scanf("%s",&bk[i].publish);
		printf("请输入该�??的出版年/月/日（以空格隔开）：\n",i);
		scanf("%d %d %d",&bk[i].t.year,&bk[i].t.month,&bk[i].t.day);
		printf("请输入该书是�??可借（1/0）：\n",&i);
		scanf("%d",&bk[i].jie);
		printf("修�??书籍信息成功\n");

		printf("修改后的书籍信息如下：\n");
		printf("编号：%d\n",bk[i].num);
		printf("书名：%s\n",bk[i].name);
		printf("出版社：%s\n",bk[i].publish);
		printf("出版日期：%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("�??否已借阅：%d\n",bk[i].jie);

	}
	else
		printf("对不起，您所�??找的书籍不存在\n");

	printf("请按任意键后退出\n");
	getchar();//缓冲，任意键后再退出

}


void xiugaimima()//修改密码
{
	char b[20],c[20];
	printf("*****************************************************\n");
	printf("*********************修改密码************************\n");
	printf("请输入原始密码：\n");
	scanf("%s",b);
	if (!strcmp(b,mima))
	{
		printf("请输入新密码：\n");
		scanf("%s",b);
		printf("请重新输入新密码：\n");
		scanf("%s",c);
		if(!strcmp(b,c))
		{
			strcpy(mima,b);
			printf("密码�??置成功！新密码为:%s\n",mima);
		}
		else
			printf("两次输入�??密码不一致，密码设置失败！\n");

	}
	else
		printf("密码输入�??误！\n");

	printf("请按任意键后退出\n");
	getchar();
}


int peo()//用户界面函数
{
	int a;
	printf("*****************欢迎进鍏????者系统******************\n");
	printf("                 请�??择操作：                        \n");
	printf("                 1、查看书籍信息\n");
	printf("                 2、借阅图书\n");
	printf("                 3、退出系统\n");
	scanf("%d",&a);
	return a;
}


void jieyue()//借阅书籍函数
{
	int i;
	char s[20];
	printf("请输入所要借阅的书籍名字：\n");
//	gets(s);//同样�??导致程序运行时界面只是一闪而过，且无法正常输入
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("查鎵????，请稍候......");
	}
	if (i<N)
		{
			printf("您要借阅的书�??信息如下：\n");
			printf("编号：%d\n",bk[i].num);
			printf("书名：%s\n",bk[i].name);
			printf("出版社：%s\n",bk[i].publish);
			printf("出版日期：%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
			printf("是否已借阅：%d\n",bk[i-1].jie);

			if(bk[i-1].jie)
				printf("对不起，该书已借出。\n");
			else 
				{
					bk[i-1].jie=1;
					printf("借阅成功！\n");
				}
		}
	else
		printf("对不起，您所查找的书籍不存在。\n");


	printf("请按任意键后退出\n");
	getchar();

}
