/*
 *  ԭ�ͣ�extern void *realloc(void *mem_address, unsigned int newsize);
 *  �﷨��ָ����=����������*��realloc��Ҫ�ı��ڴ��С��ָ�������µĴ�С����
 *        �µĴ�Сһ��Ҫ����ԭ���Ĵ�С����Ȼ�Ļ��ᵼ�����ݶ�ʧ��
 *  ͷ�ļ���#include <stdlib.h> ��Щ��������Ҫ#include <malloc.h>
 *  ���ܣ����жϵ�ǰ��ָ���Ƿ����㹻�������ռ䣬����У�����mem_addressָ��ĵ�ַ�����ҽ�mem_address���أ�
 *        ����ռ䲻�����Ȱ���newsizeָ���Ĵ�С����ռ䣬��ԭ�����ݴ�ͷ��β�������·�����ڴ�����
 *        �����ͷ�ԭ��mem_address��ָ�ڴ�����ͬʱ�����·�����ڴ�������׵�ַ�������·���洢����ĵ�ַ.
 *  ����ֵ��������·���ɹ��򷵻�ָ�򱻷����ڴ��ָ�룬���򷵻ؿ�ָ��NULL.
 *  ע�⣺����ԭʼ�ڴ��е����ݻ��Ǳ��ֲ���ġ�
 *        ���ڴ治��ʹ��ʱ��Ӧʹ��free()�������ڴ���ͷš�
*/

#include<stdio.h>
#include<stdlib.h>


#ifdef MAIN1
int main()
{
	int i;
	int *pn;

	pn =(int *)malloc(5*sizeof(int));
	if(!pn) {
		printf("malloc pn failed!\n");
		return -1;
	}
	printf("%p\n",pn);

	for(i=0;i<5;i++)
		scanf("%d",&pn[i]);

	pn=(int *)realloc(pn,10*sizeof(int));
	printf("%p\n",pn);

	for(i=0;i<5;i++)
		printf("%3d",pn[i]);
	printf("\n");

	free(pn);
	return 0;
}
#else
int main()
{
	char *p;

	p=(char *)malloc(1);
	if(p)
		printf("Memory Allocated at: %p\n",p);
	else
		printf("Not Enough Memory!\n");

	scanf("%s", p);
	puts(p);
	p=(char *)realloc(p,10);
	if(p)
		printf("Memory Reallocated at: %p\n",p);
	else
		printf("Not Enough Memory!\n");

	puts(p);
	free(p);
	return 0;
}
#endif
