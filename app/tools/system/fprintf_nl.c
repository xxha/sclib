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

	fclose(stream);

	sprintf(temp,"nl fprintf.txt");
	printf("temp = %s\n",temp);

	printf("the first system:\n");
	/* system() parameter could be (char *) or (string).*/
	system(temp);

	printf("the second system:\n");
	/* nl: bash command, output file to stdout */
	system("nl fprintf.txt");  

	return 0;

}
