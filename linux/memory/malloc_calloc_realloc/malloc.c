/*
 *  原型：extern void *malloc(unsigned int num_bytes);
 *  头文件：malloc.h或 alloc.h (注意：alloc.h 与 malloc.h 的内容是完全一致的)
 *  功能：分配长度为num_bytes字节的内存块
 *  返回值：如果分配成功则返回指向被分配内存的指针(此存储区中的初始值不确定)，否则返回空指针NULL。
 *  当内存不再使用时，应使用free()函数将内存块释放。
 *  函数返回的指针一定要适当对齐，使其可以用于任何数据对象。
 *  说明：关于该函数的原型，在旧的版本中malloc返回的是char型指针，新的ANSIC标准规定，该函数返回为void型指针，因此必要时要进行类型转换。
 *  malloc的全称是memory allocation，中文叫动态内存分配，当无法知道内存具体位置的时候，想要绑定真正的内存空间，就需要用到动态的分配内存。
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_128M_RAM 128*1024*1024

int main(int argc ,char **argv[])
{
        char *p256m = NULL;

        p256m= malloc(SIZE_128M_RAM);
	if(!p256m) {
		printf("malloc 128M memory failed!\n");
	} else {
		printf("malloc 128M memory succeed!\n");
	}

        memset(p256m, 0 , SIZE_128M_RAM);

	sleep(10);

	free(p256m);
	return 0;
}

