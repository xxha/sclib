//#include "stdafx.h"

#include"stdio.h"
//#include"conio.h"
#include"string.h"
#include"stdlib.h"
int N;
char mima[20]="mm";

//¶¨ÒåÊé¼®µÄ½á¹¹Ìå

struct book
{
    long num;
    char name[20];
    char publish[20];/*³ö°æÉç*/
    struct time
    {
        int day;
        int month;
        int year;
    }t;//t
    int jie;/*½èÔÄÓë·ñ*/
}bk[20];

int mymainmenu();//Ö÷²Ëµ¥º¯Êı
int mimayanzheng();//ÃÜÂëÑéÖ¤º¯Êı
int adm();//¹ÜÀíÔ±½çÃæº¯Êı
void xinxi();//Â¼ÈëĞÅÏ¢º¯Êı
void show();//²é¿´ĞÅÏ¢º¯Êı
void xiugai();//ĞŞ¸ÄĞÅÏ¢º¯Êı
void xiugaimima();//ĞŞ¸ÄÃÜÂëº¯Êı
int peo();//ÓÃ»§½çÃæº¯Êı
void jieyue();//½èÔÄÊé¼®º¯Êı
void show_all();//²é¿´ËùÓĞÊé¼®ĞÅÏ¢
void show_name();//ÒÀ¾İÊéÃû²é¿´Êé¼®ĞÅÏ¢


//Ö÷º¯Êı
void main()
{
int x,x2,s;//sÓÃÀ´ÅĞ¶ÏÃÜÂëÑéÖ¤½á¹û0/1
do{
	system("cls");
	x=mymainmenu();//Ö÷²Ëµ¥º¯Êı***
	switch(x)
	{
	case 1: system("cls"); //½øÈë¹ÜÀíÔ±Ä£Ê½
			s=mimayanzheng();//ÃÜÂëÑéÖ¤º¯Êı£¬ÆäÖĞ°üº¬½çÃæÏÔÊ¾***
			do
			{
				if(s==1)
				{	system("cls");
					x2=adm();//¹ÜÀíÔ±½çÃæº¯Êı£¬°üº¬½ÓÊÕÖ¸Áî***
					
						switch(x2)
						{
							case 1:system("cls");xinxi();break;//Â¼ÈëĞÅÏ¢º¯Êı***
							case 2:system("cls");show();break;//²é¿´ĞÅÏ¢º¯Êı***
							case 3:system("cls");xiugai();break;//ĞŞ¸ÄĞÅÏ¢º¯Êı***
							case 4:system("cls");xiugaimima();break;//ĞŞ¸ÄÃÜÂëº¯Êı***
						}
					
				}
				else 
					{
						printf("ÃÜÂëÊäÈë´íÎó£¡");	
						printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
						getchar();
						break;}
			}while(x2!=5);
			break;//

	case 2:		do
				{
					system("cls");//½øÈëÓÃ»§½çÃæ
					x2=peo();//ÓÃ»§½çÃæº¯Êı***
					switch(x2)
					{
					case 1:system("cls");show();break;//²é¿´Í¼Êéº¯Êı***
					case 2:system("cls");jieyue();break;//½èÔÄÍ¼Êéº¯Êı***
					}
				}while(x2!=3);
				break;
	}
}while(x!=3);
	system("cls");
	printf("thank you for your coming!\n");
	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();

}

int mymainmenu()//Ö÷²Ëµ¥º¯Êı
{
	int a;
	printf("*****************************************************\n");
	printf("*****************»¶Ó­½øÈëÍ¼Êé¹ÜÀíÏµÍ³****************\n");
	printf("                 ÇëÑ¡Ôñ²Ù×÷£º                        \n");
	printf("                 1¡¢ÒÔ¹ÜÀíÔ±Éí·İ½øÈë\n");
	printf("                 2¡¢ÒÔ¶ÁÕßÉí·İ½øÈë\n");
	printf("                 3¡¢ÍË³öÏµÍ³\n");
	scanf("%d",&a);//´íÎó£ºscanf("%d\n",&a);
	return a;
}
///*
int mimayanzheng()//ÃÜÂëÑéÖ¤º¯Êı
{
	char b[20];
	printf("*****************ÇëÊäÈëÃÜÂë£º************************\n");
//	gets(b);  //ÎªºÎÓÃgets(b)Ñ­»·Ìå¾Í³ö´í£¿Ö»ÊÇ¿ìËÙÉÁ¹ıÃÜÂëÑéÖ¤µÄ½çÃæ??????????????????????????????????
    scanf("%s",b);
	return (!strcmp(mima,b));

}
//*/

int adm()//¹ÜÀíÔ±½çÃæº¯Êı
{
	int a;
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************»¶Ó­½øÈë¹ÜÀíÔ±ÏµÍ³******************\n");
	printf("                 ÇëÑ¡Ôñ²Ù×÷£º                        \n");
	printf("                 1¡¢²åÈëÊé¼®ĞÅÏ¢\n");
	printf("                 2¡¢²é¿´Êé¼®ĞÅÏ¢\n");
	printf("                 3¡¢ĞŞ¸ÄÊé¼®ĞÅÏ¢\n");
	printf("                 4¡¢ĞŞ¸ÄÃÜÂë\n");
	printf("                 5¡¢ÍË³öÏµÍ³\n");
	scanf("%d",&a);
	return a;
}

void xinxi()//Â¼ÈëĞÅÏ¢º¯Êı
{
	int i;
	printf("*****************************************************\n");
	printf("*****************²åÈëÊé¼®ĞÅÏ¢************************\n");
	printf("ÇëÊäÈë²åÈëÊé¼®×ÜÊı£º\n");
	scanf("%d",&N);
	for(i=1;i<=N;i++)
	{
		printf("ÇëÊäÈëµÚ%d±¾ÊéµÄ±àºÅ£º\n",i);
		scanf("%d",&bk[i-1].num);
		printf("ÇëÊäÈëµÚ%d±¾ÊéµÄÃû×Ö£º\n",i);
		scanf("%s",&bk[i-1].name);
		printf("ÇëÊäÈëµÚ%d±¾ÊéµÄ³ö°æÉç£º\n",i);
		scanf("%s",&bk[i-1].publish);
		printf("ÇëÊäÈëµÚ%d±¾ÊéµÄ³ö°æÄê/ÔÂ/ÈÕ£¨ÒÔ¿Õ¸ñ¸ô¿ª£©£º\n",i);
		scanf("%d %d %d",&bk[i-1].t.year,&bk[i-1].t.month,&bk[i-1].t.day);
		printf("ÇëÊäÈëµÚ%d±¾ÊéÊÇ·ñ¿É½è£¨1/0£©£º\n",i);
		scanf("%d",&bk[i-1].jie);
		printf("²åÈëÊé¼®³É¹¦\n");
	}
	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();

}

void show_all()  //²é¿´ËùÓĞÊé¼®º¯Êı
{
	int i;
	for(i=0;i<N;i++)
	{
		printf("µÚ%d±¾ÊéµÄĞÅÏ¢£º\n",i+1);
		printf("±àºÅ£º%d\n",bk[i].num);
		printf("ÊéÃû£º%s\n",bk[i].name);
		printf("³ö°æÉç£º%s\n",bk[i].publish);
		printf("³ö°æÈÕÆÚ£º%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("ÊÇ·ñÒÑ½èÔÄ£º%d\n",bk[i-1].jie);
	}
	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();
}

void show_name() //ÒÀ¾İÊéÃû²é¿´Êé¼®ĞÅÏ¢
{
	int i;
	char s[20];
	printf("ÇëÊäÈëËùÒª²éÕÒµÄÊé¼®Ãû×Ö£º\n");
//	gets(s);//ä??ºÎ²»ĞĞ£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿£¿
	scanf("%d",s);
	for(i=0;(i<N)&&(!strcmp(s,bk[i].name));i++)
	{
		printf("²éÕÒÖĞ£¬ÇëÉÔºò......");
	}
	if (i<N)
	{
		printf("ÄúÒªæŸ????µÄÊé¼®ĞÅÏ¢ÈçÏÂ£º\n");
		printf("±àºÅ£º%d\n",bk[i].num);
		printf("ÊéÃû£º%s\n",bk[i].name);
		printf("³ö°æÉç£º%s\n",bk[i].publish);
		printf("³ö°æÈÕÆÚ£º%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("ÊÇ·ñÒÑ½èÔÄ£º%d\n",bk[i-1].jie);
	}
	else
		printf("¶Ô²»Æğ£¬ÄúËù²éÕÒµÄÊé¼®²»´æÔÚ\n");

	printf("è¯????ÈÎÒâ¼üºóÍË³ö\n");
	getchar();//»º³å£¬ÈÎÒâ¼üºóÔÙÍË³ö
}


void show()//²é¿´ĞÅÏ¢º¯Êı//ÒÔÒÀ¾İÊé¼®Ãû×Ö²é¿´ĞÅÏ¢ÎªÀı
{
	int a;
	printf("*****************************************************\n");
	printf("*****************²é¿´Êé¼®ĞÅÏ¢************************\n");
	printf("ÇëÑ¡Ôñ²Ù×÷£º\n");
	printf("1¡¢²é¿´ËùÓĞÊé¼®ĞÅÏ¢\n");
	printf("2¡¢ÒÀ¾İÊéÃû²é¿´Êé¼®ĞÅÏ¢\n");
	printf("3¡¢·µ»ØÉÏÒ»ç??²Ëµ¥\n");
	scanf("%d",&a);
	switch(a)//×¢Òâ¹Û²ìÎ´¼ÓÑ­»·µÄswitchµÄÔËĞĞ£¬Ñ­»·ÊÇÎªÁË·µ»ØÉÏÒ»¼¶²Ëµ¥
	{
	case 1:show_all();break;//²é¿´ËùÓĞÊé¼®º¯Êı****
	case 2:show_name();break;//ÒÀ¾İÊéÃû²éçœ????¼®ĞÅÏ¢º¯Êı****
	}
	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();//»º³å£¬ÈÎÒâ¼üºóÔÙÍË³ö
}


void xiugai()//ĞŞ¸ÄÊé¼®ĞÅÏ¢
{
	int i;
	char s[20];
	printf("ÇëÊäÈëËùÒªĞŞ¸ÄµÄÊé¼®Ãû×Ö£º\n");
//	gets(s);
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("²éÕÒÖĞ£¬ÇëÉÔºò......");
	}
	if (i<N)
	{
		printf("ÄúÒª²éÕÒµÄÊé¼®ĞÅÏ¢ÈçÏÂ£º\n");
		printf("±àºÅ£º%d\n",bk[i].num);
		printf("ÊéÃû£º%s\n",bk[i].name);
		printf("³ö°æÉç£º%s\n",bk[i].publish);
		printf("³ö°æÈÕÆÚ£º%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("ÊÇ·ñÒÑ½èÔÄ£º%d\n",bk[i].jie);

		printf("ÇëÖØĞÂÊäÈëÊé¼®ĞÅÏ¢£º\n");
		printf("ÇëÊäÈë¸ÃÊéµÄ±àå??£º\n");
		scanf("%d",&bk[i].num);
		printf("ÇëÊäÈë¸ÃÊéµÄÃû×Öï??\n",i);
		scanf("%s",&bk[i].name);
		printf("ÇëÊäÈë¸ÃÊéµÄ³öç‰????£º\n",i);
		scanf("%s",&bk[i].publish);
		printf("ÇëÊäÈë¸Ãä??µÄ³ö°æÄê/ÔÂ/ÈÕ£¨ÒÔ¿Õ¸ñ¸ô¿ª£©£º\n",i);
		scanf("%d %d %d",&bk[i].t.year,&bk[i].t.month,&bk[i].t.day);
		printf("ÇëÊäÈë¸ÃÊéÊÇå??¿É½è£¨1/0£©£º\n",&i);
		scanf("%d",&bk[i].jie);
		printf("ĞŞæ??Êé¼®ĞÅÏ¢³É¹¦\n");

		printf("ĞŞ¸ÄºóµÄÊé¼®ĞÅÏ¢ÈçÏÂ£º\n");
		printf("±àºÅ£º%d\n",bk[i].num);
		printf("ÊéÃû£º%s\n",bk[i].name);
		printf("³ö°æÉç£º%s\n",bk[i].publish);
		printf("³ö°æÈÕÆÚ£º%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
		printf("æ??·ñÒÑ½èÔÄ£º%d\n",bk[i].jie);

	}
	else
		printf("¶Ô²»Æğ£¬ÄúËùæ??ÕÒµÄÊé¼®²»´æÔÚ\n");

	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();//»º³å£¬ÈÎÒâ¼üºóÔÙÍË³ö

}


void xiugaimima()//ĞŞ¸ÄÃÜÂë
{
	char b[20],c[20];
	printf("*****************************************************\n");
	printf("*********************ĞŞ¸ÄÃÜÂë************************\n");
	printf("ÇëÊäÈëÔ­Ê¼ÃÜÂë£º\n");
	scanf("%s",b);
	if (!strcmp(b,mima))
	{
		printf("ÇëÊäÈëĞÂÃÜÂë£º\n");
		scanf("%s",b);
		printf("ÇëÖØĞÂÊäÈëĞÂÃÜÂë£º\n");
		scanf("%s",c);
		if(!strcmp(b,c))
		{
			strcpy(mima,b);
			printf("ÃÜÂëè??ÖÃ³É¹¦£¡ĞÂÃÜÂëÎª:%s\n",mima);
		}
		else
			printf("Á½´ÎÊäÈëç??ÃÜÂë²»Ò»ÖÂ£¬ÃÜÂëÉèÖÃÊ§°Ü£¡\n");

	}
	else
		printf("ÃÜÂëÊäÈëæ??Îó£¡\n");

	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();
}


int peo()//ÓÃ»§½çÃæº¯Êı
{
	int a;
	printf("*****************»¶Ó­½øå…????ÕßÏµÍ³******************\n");
	printf("                 Çëé??Ôñ²Ù×÷£º                        \n");
	printf("                 1¡¢²é¿´Êé¼®ĞÅÏ¢\n");
	printf("                 2¡¢½èÔÄÍ¼Êé\n");
	printf("                 3¡¢ÍË³öÏµÍ³\n");
	scanf("%d",&a);
	return a;
}


void jieyue()//½èÔÄÊé¼®º¯Êı
{
	int i;
	char s[20];
	printf("ÇëÊäÈëËùÒª½èÔÄµÄÊé¼®Ãû×Ö£º\n");
//	gets(s);//Í¬Ñùä??µ¼ÖÂ³ÌĞòÔËĞĞÊ±½çÃæÖ»ÊÇÒ»ÉÁ¶ø¹ı£¬ÇÒÎŞ·¨Õı³£ÊäÈë
	scanf("%s",s);
	for(i=0;(i<N)&&(strcmp(s,bk[i].name));i++)
	{
		printf("²éæ‰????£¬ÇëÉÔºò......");
	}
	if (i<N)
		{
			printf("ÄúÒª½èÔÄµÄÊéç??ĞÅÏ¢ÈçÏÂ£º\n");
			printf("±àºÅ£º%d\n",bk[i].num);
			printf("ÊéÃû£º%s\n",bk[i].name);
			printf("³ö°æÉç£º%s\n",bk[i].publish);
			printf("³ö°æÈÕÆÚ£º%d/%d/%d\n",bk[i].t.year,bk[i].t.month,bk[i].t.day);
			printf("ÊÇ·ñÒÑ½èÔÄ£º%d\n",bk[i-1].jie);

			if(bk[i-1].jie)
				printf("¶Ô²»Æğ£¬¸ÃÊéÒÑ½è³ö¡£\n");
			else 
				{
					bk[i-1].jie=1;
					printf("½èÔÄ³É¹¦£¡\n");
				}
		}
	else
		printf("¶Ô²»Æğ£¬ÄúËù²éÕÒµÄÊé¼®²»´æÔÚ¡£\n");


	printf("Çë°´ÈÎÒâ¼üºóÍË³ö\n");
	getchar();

}
