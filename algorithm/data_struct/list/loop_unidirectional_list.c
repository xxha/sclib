/*
 * print loop link.
 * change £¨NULL != pLinkNode£©to£¨pLinkNode != pIndex£©¡£
 */
void print_data(const LINK_NODE* pLinkNode)
{
	LINK_NODE* pIndex = NULL;
	if(NULL == pLinkNode)
		return;

	printf("%d\n", pLinkNode->data);
	pIndex = pLinkNode->next;
	while(pLinkNode != pIndex){
		printf("%d\n", pIndex->data);
		pIndex = pIndex ->next;
	}
}

/* insert data to loop link */
STATUS insert_data(LINK_NODE** ppLinkNode, int data)
{
	LINK_NODE* pNode;
	if(NULL == ppLinkNode)
		return FALSE;

	if(NULL == *ppLinkNode){
		pNode = create_link_node(data);
		assert(NULL != pNode);

		pNode->next = pNode;
		*ppLinkNode = pNode;
		return TRUE;
	}

	if(NULL != find_data(*ppLinkNode, data))
		return FALSE;

	pNode = create_link_node(data);
	assert(NULL != pNode);

	pNode->next = (*ppLinkNode)->next;
	(*ppLinkNode)->next = pNode;
	return TRUE;
}

/* delete data */
STATUS delete_data(LINK_NODE** ppLinkNode, int data)
{
	LINK_NODE* pIndex = NULL;
	LINK_NODE* prev = NULL;
	if(NULL == ppLinkNode || NULL == *ppLinkNode)
		return FALSE;

	pIndex = find_data(*ppLinkNode, data);
	if(NULL == pIndex)
		return FALSE;

	if(pIndex == *ppLinkNode){
		if(pIndex == pIndex->next){
			*ppLinkNode = NULL;
		}else{
			prev = pIndex->next;
			while(pIndex != prev->next)
				prev = prev->next;

			prev->next = pIndex->next;
			*ppLinkNode = pIndex->next;
		}
	}else{
		prev = pIndex->next;
		while(pIndex != prev->next)
			prev = prev->next;
		prev->next = pIndex->next;
	}

	free(pIndex);
	return TRUE;
}




