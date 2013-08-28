#include <iostream>

using namespace std;

struct Node {
	char data;  //for 0-9, 1 byte is enough
	Node *next;
};

//version 1
Node* AddSinglyLinkedList(Node *list1, Node *list2) {
	Node *q1 = list1, *q2 = list2;
	Node *ans = NULL, *p = NULL, *pre = NULL, *q =NULL;   //pre means the node before q
	bool highest = false;       //the highest bit maybe inc

	//process the highest node
	char first = q1->data + q2->data;
	if(first >= 9) { // sum >= 9
		highest = true;
		ans = new Node();
		pre = new Node();
		pre->data = first % 10;
		ans->next = pre;
		if(first > 9) { //sum > 9
			ans->data = 1;
			p = pre;
		} else {        // sum = 9
			ans->data = 0;
			p = ans;
		}
	} else { //sum < 9
		ans = new Node();
		ans->data = first;
		p = pre = ans;
	}

	//process following nodes
	while((q1=q1->next) && (q2 = q2->next)) {
		q = new Node();      	//current output node
		pre->next = q;  	
		char num = q1->data + q2->data;
		q->data = num % 10;

		if(num > 9) {
			p->data = p->data + 1; 
			for(p = p->next; p!=q; p=p->next)
				p->data = 0;
		} else if(num < 9) {
			p = q;
		}
		pre = q;
	}
	if(highest && ans->data == 0)
		ans = ans->next;

	return ans;
}

Node *MakeLinkedList(int d[], int n) {
	Node *head = new Node();
	head->data = (char)d[0];
	Node *pre = head, *cur = head;

	for(int i=1; i<n; ++i) {
		cur = new Node();
		cur->data = (char)d[i];
		pre->next = cur;
		pre = cur;
	}
	return head;
}

//version 2
Node *AddSinglyLinkedList1(Node *list1, Node *list2) {
	Node *q1 = list1, *q2 = list2;
	Node *ans = NULL, *p = NULL, *pre = NULL, *q = NULL;

	//precoess the highest node
	char first = q1->data + q2->data;
	ans = new Node();
	ans->data = first;
	p = pre = ans;

	while((q1=q1->next) && (q2=q2->next)) {
		q = new Node();
		pre->next = q;
		char num = q1->data + q2->data;
		q->data = num % 10;
		if(num > 9) {
			p->data = p->data + 1;
			for(p=p->next; p!=q; p=p->next)
				p->data = 0;
		} else if(num < 9) {
			p = q;
		}
		pre = q;
	}

	if(ans->data > 9) {
		q = new Node();
		q->data = 1;
		ans->data = ans->data - 10;
		q->next = ans;
		ans = q;
	}

	return ans;
}

int main(){
	int n = 7;
	int a[] = {2, 0, 0, 0, 7, 0, 1};
	int b[] = {7, 9, 9, 9, 9, 9, 9};

	Node *list1 = MakeLinkedList(a, n);
	Node *list2 = MakeLinkedList(b, n);

	Node *ans = AddSinglyLinkedList(list1, list2);
	for(; ans; ans=ans->next)
		cout<<(int)ans->data;
	cout<<endl;

	Node *ans1 = AddSinglyLinkedList1(list1, list2);
	for(;ans1; ans1=ans1->next)
		cout<<(int)ans1->data;
	cout<<endl;

	return 0;
}



























