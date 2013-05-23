/*
 *   void *calloc(unsigned n,unsigned size)��
 *   �� ��: ���ڴ�Ķ�̬�洢���з���n������Ϊsize�������ռ䣬��������һ��ָ�������ʼ��ַ��ָ�룻������䲻�ɹ�������NULL��
 *   ��malloc������calloc�ڶ�̬�������ڴ���Զ���ʼ�����ڴ�ռ�Ϊ�㣬��malloc����ʼ�������������������������ݡ�
 *   ����malloc������ڴ�����ǲ������ģ���calloc������ڴ�ռ�����������ġ�
 *   ͷ�ļ���stdlib.h��malloc.h
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#define MAIN1
#define MAIN2

#ifdef MAIN1
int main(void)
{
	char *str = NULL;

	/* �����ڴ�ռ� */
	str = (char*)calloc(10, sizeof(char));

	/* ��helloд��*/
	strcpy(str, "Hello");

	/*��ʾ��������*/
	printf("String is %s\n", str);

	/* �ͷſռ� */
	free(str);
	return 0;
}
#endif
#ifdef MAIN2
int main(void)
{
	int i;

	int *pn=(int *)calloc(10,sizeof(int));

	for(i=0;i<10;i++)
		printf("%3d",pn[i]);

	printf("\n");

	free(pn);
	return 0;
}

#endif
