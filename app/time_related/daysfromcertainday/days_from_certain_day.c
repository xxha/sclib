#include <stdio.h>

int getDayOfYear(int year, int month, int day);
int main()
{
	int y, m, d;
	int i;
	const int Y = 2014, M = 1, D = 31, Mmax = 12, Dmax = 31;
	while(1)
	{
		int sum = 0;
		scanf("%d-%d-%d", &y, &m, &d);
						
		if(Y - y == 0)	//同年
		{
			if(m >= M)	//2014-1-31 到 y-m-d
			{
				printf("%d\n", getDayOfYear(y, m, d) - getDayOfYear(2014, 1, 31));
			
			}
			else		//y-m-d 到 2014-1-31
			{
				printf("%d\n", getDayOfYear(y, m, d) - getDayOfYear(2014, 1, 31));
			
			}
		}
		else	//不同年
		{
			if(y > Y)   //2014-1-31 到 y-m-d
			{
				sum = getDayOfYear(2014, 12, 31) - getDayOfYear(2014, 1, 31); //2014-1-31 到 2014-12-31
				for(i = 2014 + 1; i < y; ++i)								  //2015-1-1 到 y-1 -12-31
				{
					sum += getDayOfYear(i, 12, 31);
				}
				sum += getDayOfYear(y, m, d);								  //y -1 -1 到 y -m -d
				printf("%d\n", sum);
			}
			else		//y-m-d 到 2014-1-31
			{
				sum = getDayOfYear(y, 12, 31) - getDayOfYear(y, m, d);		  //y -m -d 到 y -12 -31
				for(i = y+1; i < 2014; ++i)
				{
					sum += getDayOfYear(i, 12, 31);							  //y+1 -1 -1 到 2014-12-31
				}
				sum += getDayOfYear(2014, 1, 31);							  //2014-1-1 到 2014-1-31
				printf("%d\n", sum);
			}
			
		}
		printf("Try again:\n");
	}
}


int getDayOfYear(int year, int month, int day)
{
	int i;
	int days[2][13] = {	// 0 1 2 3 4 5 6 7 8 9 10 11 12
				{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
				{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
			  };

	int flag =(year%4==0 && year%100!=0) || year%400 == 0 ? 1 : 0;
	int sum = day ;
	for(i=0;i<month;i++)	{
		sum += days[flag][i];
	}		
	return sum;		
}
