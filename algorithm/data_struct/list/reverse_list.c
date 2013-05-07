#include <stdio.h>
#include <string.h>

LinkList *reverse(LinkList *head)
{
	LinkList *p1,*p2 = NULL,*p3 = NULL;
	if(head == NULL || head->next == NULL)
		return head;
	p1 = head->next;
	while(p1!=NULL)
	{
		p3 = p1->next;
		p1->next = p2;
		p2 = p1;
		p1 = p3;
	}
	head->next = p2;
	// head = p2;


	return head;
}



