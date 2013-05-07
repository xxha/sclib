#include <stdio.h>
#include <stdlib.h>

int main()
{
	int sum_day(int month,int day);
	int leap(int year);
	int year,month,day,days,flag=1;
	char c;

	while(flag)
	{
		printf("\nInput date(year-month-day):\n");
		scanf("%d-%d-%d", &year, &month, &day);
		printf("\n%d-%d-%d ", year, month, day);

		days=sum_day(month,day);

		if(leap(year) && month >= 3)
			days=days+1;

		printf("is the %d days in this year\n",days);

		printf("if not continue press N or n:\n");
		c=getchar();
		if((c=='n')||(c=='N'))
			exit(0);
		else
			flag=1;
	}
   
}
int sum_day(int month,int day)
{
	int day_month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	for(i=1;i<month;i++)
		day=day+day_month[i];
	return(day);
	}
int leap(int year)
{
	int leap=0;
	if((year % 4 == 0 && year % 100 != 0)||(year % 400 == 0))
		leap=1;
	return(leap);
	}
