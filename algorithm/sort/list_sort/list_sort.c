
void sort_for_link_node(NODE** ppNode)
{
	NODE* prev;
	NODE* curr;

	if(NULL == ppNode || NULL == *ppNode)
		return;

	curr = (*ppNode) ->next;
	(*ppNode) ->next = NULL;

	while(curr){
		prev = curr;
		curr = curr->next;
		insert_for_sort_operation(ppNode, prev);
	}

	return;
}

void insert_for_sort_operation(NODE** ppNode, NODE* pNode)
{
	NODE* prev;
	NODE* cur;

	if(pNode->data < (*ppNode)->data){
		pNode->next = *ppNode;
		*ppNode = pNode;
		return;
	}

	cur = *ppNode;
	while(cur){
		if(pNode->data < cur->data)
			break;

		prev = cur;
		cur = cur->next;
	}

	pNode->next = prev->next;
	prev->next = pNode;
	return;
}

