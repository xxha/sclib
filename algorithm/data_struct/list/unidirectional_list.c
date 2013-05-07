
/* link data structure */
typedef struct _LINK_NODE
{
	int data;
	struct _LINK_NODE* next;
}LINK_NODE;

/* create link */
LINK_NODE* alloca_node(int value)
{
	LINK_NODE* pLinkNode = NULL;
	pLinkNode = (LINK_NODE*)malloc(sizeof(LINK_NODE));
	
	pLinkNode->data = value;
	pLinkNode->next = NULL;
	return pLinkNode;
}

/* delete link */
void delete_node(LINK_NODE** pNode)
{
    LINK_NODE** pNext;
    if(NULL == pNode || NULL == *pNode)
	    return ;
		
	pNext = &(*pNode)->next;
	free(*pNode);
	delete_node(pNext);	
}

/* insert data to link */
STATUS _add_data(LINK_NODE** pNode, LINK_NODE* pDataNode)
{
    if(NULL == *pNode){
	    *pNode = pDataNode;
		return TRUE;
	}
	
	return _add_data(&(*pNode)->next, pDataNode); //把新添加的链表放到最后
}

STATUS add_data(const LINK_NODE** pNode, int value)
{
    LINK_NODE* pDataNode;
    if(NULL == *pNode)
	    return FALSE;
		
	pDataNode = alloca_node(value);
	assert(NULL != pDataNode);
	return _add_data((LINK_NODE**)pNode, pDataNode);
}

/* delete data in link */
STATUS _delete_data(LINK_NODE** pNode, int value)
{
    LINK_NODE* pLinkNode;
    if(NULL == (*pNode)->next)
	    return FALSE;
	
	pLinkNode = (*pNode)->next;
	if(value == pLinkNode->data){
	    (*pNode)->next = pLinkNode->next;
		free(pLinkNode);
		return TRUE;
	}else{
	    return _delete_data(&(*pNode)->next, value);
	}
}

STATUS delete_data(LINK_NODE** pNode, int value)
{
    LINK_NODE* pLinkNode;
    if(NULL == pNode || NULL == *pNode)
	    return FALSE;

    if(value == (*pNode)->data){
	    pLinkNode = *pNode;
		*pNode = pLinkNode->next;
		free(pLinkNode);
		return TRUE;
	}		
	
	return _delete_data(pNode, value);
}

/* find data in link */
LINK_NODE* find_data(const LINK_NODE* pLinkNode, int value)
{
    if(NULL == pLinkNode)
	    return NULL;
	
	if(value == pLinkNode->data)
	    return (LINK_NODE*)pLinkNode;
	
	return find_data(pLinkNode->next, value);
}

/* print data in link */
void print_node(const LINK_NODE* pLinkNode)
{
    if(pLinkNode){
	    printf("%d\n", pLinkNode->data);
		print_node(pLinkNode->next);
	}
}

/* count data number in link */
int count_node(const LINK_NODE* pLinkNode)
{
    if(NULL == pLinkNode)
	    return 0;
		
	return 1 + count_node(pLinkNode->next);
}


