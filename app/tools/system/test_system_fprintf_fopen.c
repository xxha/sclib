#include<stdio.h>
#include<stdlib.h>

int main()
{
	FILE *stream;
	char temp[255];
	char *a="hello,c!";

	stream=fopen("fprintf.txt","w");
	fprintf(stream,"%s\n",a);
	fprintf(stream,"sb,c++\n");
	sprintf(temp,"nl fprintf.txt");

	fclose(stream);

	printf("temp = %s\n",temp);

	printf("the first system:\n");
	system(temp);//system������������Ϊ�ַ���ָ���ֱ���ַ���

	printf("the second system:\n");
	system("nl fprintf.txt");  //nl ��bash����

	exit(0);
	return 0;

}
