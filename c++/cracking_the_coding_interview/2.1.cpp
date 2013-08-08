/*
 * Write code to remove duplicates from an unsorted linked list.
 * FOLLOW UP
 * How would you solve this problem if a temporary buffer is not allowed?
*/

#include <iostream>
#include <cstring>

using namespace std;

typedef struct node {
	int data;
	node *next;

}node;

bool hash[100];

/* initialize a list from array data */
node *init(int a[], int n)
{
	node *head, *p;

	for(int i=0; i<n; ++i) {
		node *nd = new node();
		nd->data = a[i];
		if(i==0) {
			head = p = nd;
			continue;
		}
		p->next = nd;
		p = nd;
	}
	return head;
}

/* use hase buffer */
void removeduplicate(node *head)
{
	if(head == NULL)
		return;

	node *p = head, *q = head->next;
	hash[head->data] = true;

	while(q) {
		if (hash[q->data]) {
			node *t = q;
			p->next = q->next;
			q = p->next;
			delete t;
		} else {
			hash[q->data] = true;
			p = q;
			q = q->next;
		}
	}
}

/* without hash buffer */
void removeduplicate1(node *head)
{
	if(head == NULL)
		return;

	node *p, *q, *c = head;

	while(c) {
		p = c;
		q = c->next;
		int d = c->data;
		while(q) {
			if (q->data == d) {
				node *t = q;
				p->next = q->next;
				q = p->next;
				delete t;
			} else {
				p = q;
				q = q->next;
			}
		}
		c = c->next;
	}
}

void print(node *head)
{
	while(head) {
		cout<<head->data<<" ";
		head = head->next;
	}
	cout<<endl;
}

int main()
{
	int n = 10;
	int a[] = { 3, 2, 1, 3, 5, 6, 2, 6, 3, 1 };
	int b[] = { 3, 2, 1, 3, 5, 6, 2, 6, 3, 1 };

	memset(hash, false, sizeof(hash));
	node *head = init(a, n);
	node *headb = init(b, n);

	removeduplicate1(head);
	removeduplicate(headb);
	print(head);
	print(headb);
}

