/*
Hash table. is a middle struct between list and binary tree.
List is very convenient, but not for find action.
Binary tree add a pointer to make data in sequence.
Hash table make the data convenient to find and also don't occupy too much space.
*/

/* define hash table and basic data node. */
typedef struct _NODE
{
	int data;
	struct _NODE* next;
}NODE;

typedef struct _HASH_TABLE
{
	NODE* value[10];
}HASH_TABLE;

/* create hash table */
HASH_TABLE* create_hash_table()
{
	HASH_TABLE* pHashTbl = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
	memset(pHashTbl, 0, sizeof(HASH_TABLE));
	return pHashTbl;
}

/* find data in hash table */
NODE* find_data_in_hash(HASH_TABLE* pHashTbl, int data)
{
	NODE* pNode;
	if(NULL ==  pHashTbl)
		return NULL;

	if(NULL == (pNode = pHashTbl->value[data % 10]))
		return NULL;

	while(pNode){
		if(data == pNode->data)
			return pNode;
		pNode = pNode->next;
	}
	return NULL;
}

/* insert data to hash table */
STATUS insert_data_into_hash(HASH_TABLE* pHashTbl, int data)
{
	NODE* pNode;
	if(NULL == pHashTbl)
		return FALSE;

	if(NULL == pHashTbl->value[data % 10]){
		pNode = (NODE*)malloc(sizeof(NODE));
		memset(pNode, 0, sizeof(NODE));
		pNode->data = data;
		pHashTbl->value[data % 10] = pNode;
		return TRUE;
	}

	if(NULL != find_data_in_hash(pHashTbl, data))
		return FALSE;

	pNode = pHashTbl->value[data % 10];
	while(NULL != pNode->next)
		pNode = pNode->next;

	pNode->next = (NODE*)malloc(sizeof(NODE));
	memset(pNode->next, 0, sizeof(NODE));
	pNode->next->data = data;
	return TRUE;
}

/* delete data in a hash table */
STATUS delete_data_from_hash(HASH_TABLE* pHashTbl, int data)
{
	NODE* pHead;
	NODE* pNode;
	if(NULL == pHashTbl || NULL == pHashTbl->value[data % 10])
		return FALSE;

	if(NULL == (pNode = find_data_in_hash(pHashTbl, data)))
		return FALSE;

	if(pNode == pHashTbl->value[data % 10]){
		pHashTbl->value[data % 10] = pNode->next;
		goto final;
	}

	pHead = pHashTbl->value[data % 10];
	while(pNode != pHead ->next)
		pHead = pHead->next;
	pHead->next = pNode->next;

final:
	free(pNode);
	return TRUE;
}
/*
summary:
	1. hash table is not complicated, we will use it frequently.
	2. hash table can be combined with binary tree to a complicated data.
*/
