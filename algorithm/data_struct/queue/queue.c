/*
 *   queue feature:
 *
 *   £¨1£©head and tail
 *   £¨2£©push data into tail
 *   £¨3£©pull data from head.
 *
*/

/* queue data structure */

typedef struct _QUEUE_NODE
{
	int* pData;   /* data area */
	int length;   /* the length of pData */
	int head ;    /* queue head */
	int tail;     /* queue tail */
	int count;    /* queue element number */
}QUEUE_NODE;

/* allocate queue memory */
QUEUE_NODE* alloca_queue(int number)
{
	QUEUE_NODE* pQueueNode;
	if( 0 == number)
		return NULL;

	pQueueNode = (QUEUE_NODE*)malloc(sizeof(QUEUE_NODE));
	assert(NULL != pQueueNode);
	memset(pQueueNode, 0, sizeof(QUEUE_NODE));

	pQueueNode->pData = (int*)malloc(sizeof(int) * number);
	if(NULL == pQueueNode->pData){
		free(pQueueNode);
		return NULL;
	}

	pQueueNode->length = number;
	return pQueueNode;
}

/* free queue memory */
STATUS delete_queue(const QUEUE_NODE* pQueueNode)
{
	if(NULL == pQueueNode) 
		return FALSE;
	
	assert(NULL != pQueueNode->pData);
	
	free(pQueueNode->pData);
	free((void*)pQueueNode);
	return TRUE;
}

/* push data to a queue */
STATUS insert_queue(QUEUE_NODE* pQueueNode, int value)
{
	if(NULL == pQueueNode)
		return FALSE;

	if(pQueueNode->length == pQueueNode->count)
		return FALSE;

	pQueueNode->pData[pQueueNode->tail] = value;   /* push from tail */
	pQueueNode->tail = (pQueueNode->tail + 1) % pQueueNode->length;  
	pQueueNode->count ++;
	return TRUE;
}

/* pull data from queue */
STATUS get_queue_data(QUEUE_NODE* pQueueNode, int* value)
{
	if(NULL == pQueueNode || NULL == value)
		return FALSE;

	if(0 == pQueueNode->count)
		return FALSE;

	*value = pQueueNode->pData[pQueueNode->head];
	pQueueNode-> pData[pQueueNode->head] = 0; 
	pQueueNode-> count --;
	pQueueNode->head = (pQueueNode->head + 1) % pQueueNode->length;  /* pull from head */
	return TRUE;
}

/* calculate data element number in a queue */
int  get_total_number(const QUEUE_NODE* pQueueNode)
{
	if(NULL == pQueueNode)
		return 0;

	return pQueueNode->count;
}

/* calculate the whole length of queue */
int  get_total_length(const QUEUE_NODE* pQueueNode)
{
	if(NULL == pQueueNode)
		return 0;

	return pQueueNode->length;
}

