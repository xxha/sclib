/*
 *  原型：extern void *realloc(void *mem_address, unsigned int newsize);
 *  语法：指针名=（数据类型*）realloc（要改变内存大小的指针名，新的大小）。
 *        新的大小一定要大于原来的大小，不然的话会导致数据丢失！
 *  头文件：#include <stdlib.h> 有些编译器需要#include <malloc.h>
 *  功能：先判断当前的指针是否有足够的连续空间，如果有，扩大mem_address指向的地址，并且将mem_address返回，
 *        如果空间不够，先按照newsize指定的大小分配空间，将原有数据从头到尾拷贝到新分配的内存区域，
 *        而后释放原来mem_address所指内存区域，同时返回新分配的内存区域的首地址。即重新分配存储器块的地址.
 *  返回值：如果重新分配成功则返回指向被分配内存的指针，否则返回空指针NULL.
 *  注意：这里原始内存中的数据还是保持不变的。
 *        当内存不再使用时，应使用free()函数将内存块释放。
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
