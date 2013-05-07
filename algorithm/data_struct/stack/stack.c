/*
	stack feature:

	1. data first in, late out.
	2. the lengh of stack is determined by height of stack top.
*/


/* stack data structure */
typedef struct _STACK_NODE
{
	int* pData;     /*data area*/
	int length; /*stack size*/
	int top;    /* data element number, top < length. */
}STACK_NODE;

/*create stack*/
STACK_NODE* alloca_stack(int number)
{
    STACK_NODE* pStackNode = NULL;
    if(0 == number)
	    return NULL;
	
    pStackNode = (STACK_NODE*)malloc(sizeof(STACK_NODE));
    assert(NULL != pStackNode);
    memset(pStackNode, 0, sizeof(STACK_NODE));
	
    pStackNode->pData = (int*)malloc(sizeof(int) * number);
    if(NULL == pStackNode->pData){
	    free(pStackNode);
        return NULL;
    }
	
    memset(pStackNode->pData, 0, sizeof(int) * number);
    pStackNode-> length = number;
    pStackNode-> top= 0;
    return pStackNode;
}

/* free stack */
STATUS free_stack(const STACK_NODE* pStackNode)
{
    if(NULL == pStackNode)
        return FALSE;
	
    assert(NULL != pStackNode->pData);	
		
    free(pStackNode->pData);
    free((void*)pStackNode);
    return TRUE;
}

/*push data into stack*/
STATUS stack_push(STACK_NODE* pStackNode, int value)
{
    if(NULL == pStackNode)
        return FALSE;
		
    if(pStackNode->length = pStackNode->top)
        return FALSE;
		
    pStackNode->pData[pStackNode->top ++] = value;
    return TRUE;
}

/* pull data from stack*/
STATUS stack_pop(STACK_NODE* pStackNode, int* value)
{
    if(NULL == pStackNode || NULL == value)
        return FALSE;
		
    if(0 == pStackNode->top)
        return FALSE;
		
    *value = pStackNode->pData[-- pStackNode->top];
    return TRUE;
}

/* count data element number in stack */
int count_stack_number(const STACK_NODE* pStackNode)
{
    return pStackNode->top;
}

/* calculate stack size */ 
int stack_length(const STACK_NODE* pStackNode)
{
    return pStackNode->top;
}
