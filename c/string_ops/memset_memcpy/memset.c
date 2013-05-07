/*
 *  void *memset(void *s,int ch,size_t n); 
 *  �������ͣ��� s ��ǰ n ���ֽ��� ch �滻������ s 
 *  memset����������һ���ڴ�������ĳ��������ֵ�����ǶԽϴ�Ľṹ�������������������һ����췽��
 *  ͷ�ļ� #include <string.h>
 *  ����ֵ�� s
 */

/*
    void *memcpy(void *dest, const void *src, size_t n);
    ���ã� ��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ����
    ͷ�ļ�: #include <string.h>
    ��������: dest��ֵ��
*/


#include <string.h>
#include <stdio.h>

int main()
{
	char buf[30];
	char str1[10];
	char str2[20];
	char space = ' ';
	int i;

	memset(buf,0,sizeof(buf));
	printf("length of buf = %d\n", strlen(buf));
	printf("size of buf = %d\n", sizeof(buf));

	printf("please input str1 and str2\n");
	scanf("%s %s",str1, str2);
	printf("length of str1 = %d\n", strlen(str1));
	printf("length of str2 = %d\n", strlen(str2));
	printf("size of str1 = %d\n", sizeof(str1));
	printf("size of str2 = %d\n", sizeof(str2));

	memcpy(buf, str1, strlen(str1));
	memcpy(buf + strlen(str1), &space, 1);
	memcpy(buf + strlen(str1) + 1, str2, strlen(str2));

	printf("length of buf = %d\n", strlen(buf));
	printf("size of buf = %d\n", sizeof(buf));

	printf("after memcpy, buf = %s\n", buf);

	return 0;
} 

