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
	system(temp);//system函数参数可以为字符串指针或直接字符串

	printf("the second system:\n");
	system("nl fprintf.txt");  //nl 是bash命令

	exit(0);
	return 0;

}
