#include<ctype.h>
#include<stdio.h>

int main(void)
{
	char ch;
	int total = 0;

	/*ͳ����ĸ��*/
	do
	{
		ch=getchar();
		if(isalpha(ch)!=0)
		total++;
	}while(ch!='.');/*end with "."*/

	printf("The total of letters is %d \n",total);
	return 0;
}
