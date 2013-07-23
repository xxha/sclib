#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(i) (rand()%i)
#define N 15

/*维护堆的性质，这里是用的最大堆, 且为二叉堆*/
void MAX_HEAPIFY(int A[],int i,int heapSize)
{
	int l; 		/* left child*/
	int r;  	/*right child*/
	int largest; 	/* root data is the largest*/
	int temp;  
	int k;

	l=2*i+1;
	r=2*i+2;

	if((l<heapSize)&&(A[l]>A[i]))/* 如果左孩子大，那么记下下标 */
	{
		largest=l;
	}
	else
	{
		largest=i;

	}

	if ((r<heapSize)&&(A[r]>A[largest]))
	{
		largest=r;
	}

	if (largest!=i)
	{
		temp=A[i];
		A[i]=A[largest];
		A[largest]=temp;
		/* recursive call */
		MAX_HEAPIFY(A,largest,heapSize);
	}
	
}

/*build heap*/
void BUILD_HEAP(int A[]){
	int i;
	for (i=N/2-1;i>=0;i--)
	{
		MAX_HEAPIFY(A,i,N);
	}

}

/*堆排序*/
void HEAP_SORT(int A[]){
	int  i;
	int j=0;
	int temp; 	/* temp for exchange */
	int size=N; 	/* size: element number */

	/* build heap first */
	BUILD_HEAP(A);
	for(i=N-1;i>0;i--)
	{
		/* since root node always the biggest data, we exchange the root data to the correspond place
		 * to realize sort.
		 * heap sort time complexity is O（nlgn）
		 */
		temp=A[i];
		A[i]=A[0];
		A[0]=temp;
		size--;
		MAX_HEAPIFY(A,0,size);

	}
	for(j=0;j<N;j++)
	{
		printf("%5d",A[j]);
	}

}
void main(){

	int rand_no=0;
	int i=0;
	int a[N]; /* n: array length */
	srand((int)time(0)); /* set random data seed */
	printf("============================== before sort =========================================");
	printf("\n");
	for(rand_no=0;rand_no<N;rand_no++)
	{

		a[rand_no]=random(100);
		printf("%5d",a[rand_no]);
		
	}
	printf("\n");
	printf("============================== after sort =========================================\n");
	HEAP_SORT(a);
	printf("\n");
}

