/*
Tree data structure
Tree has 3 pointer, and data area.
*/
typedef struct _TREE_NODE
{
	int data;
	struct _TREE_NODE* parent;
	struct _TREE_NODE* left_child;
	struct _TREE_NODE* right_child;
}TREE_NODE;

/*
sequence binary tree:
	1. data value of all left son node < data value of this node.
	2. data value of all right son node > data value of this node.
*/

/* create a binary tree node */
TREE_NODE* create_tree_node(int data)
{
	TREE_NODE* pTreeNode = NULL;
	pTreeNode = (TREE_NODE*)malloc(sizeof(TREE_NODE));
	assert(NULL != pTreeNode);

	memset(pTreeNode, 0, sizeof(TREE_NODE));
	pTreeNode->data = data;
	return pTreeNode;
}

/* find data in tree */
TREE_NODE* find_data_in_tree_node(const TREE_NODE* pTreeNode, int data)
{
	if(NULL == pTreeNode)
		return NULL;

	if(data == pTreeNode->data)
		return (TREE_NODE*)pTreeNode;
	else if(data < pTreeNode->data)
		return find_data_in_tree_node(pTreeNode->left_child, data);
	else
		return find_data_in_tree_node(pTreeNode->right_child, data);
}


/* calculate node number in tree */
int count_node_number_in_tree(const TREE_NODE* pTreeNode)
{
	if(NULL == pTreeNode)
		return 0;

	return 1 + count_node_number_in_tree(pTreeNode->left_child)
		+ count_node_number_in_tree(pTreeNode->right_child);
}

/* sort data */
void print_all_node_data(const TREE_NODE* pTreeNode)
{
	if(pTreeNode){
		print_all_node_data(pTreeNode->left_child);
		printf("%d\n", pTreeNode->data);
		print_all_node_data(pTreeNode->right_child);
	}
}

/* calculate the height of tree */
int calculate_height_of_tree(const TREE_NODE* pTreeNode)
{
	int left, right;
	if(NULL == pTreeNode)
		return 0;

	left = calculate_height_of_tree(pTreeNode->left_child);
	right = calculate_height_of_tree(pTreeNode->right_child);
	return (left > right) ? (left + 1) : (right + 1);
}

/* judge the legality of data */
STATUS insert_node_into_tree(TREE_NODE** ppTreeNode, int data)
{
	if(NULL == ppTreeNode)
		return FALSE;

	return TRUE;
}

/* use test to varify  */ 
static void test1()
{
	assert(FALSE == insert_node_into_tree(NULL, 10));
}

/* judge the root node existance */
STATUS insert_node_into_tree(TREE_NODE** ppTreeNode, int data)
{
	if(NULL == ppTreeNode)
		return FALSE;

	if(NULL == *ppTreeNode){
		*ppTreeNode = (TREE_NODE*)create_tree_node(data);
		assert(NULL != *ppTreeNode);
		return TRUE;
	}

	return TRUE;
}

static void test2()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(10 == pTreeNode->data);
	free(pTreeNode);
}

/* consider the existance of root node*/
STATUS _insert_node_into_tree(TREE_NODE** ppTreeNode, int data, TREE_NODE* pParent)
{
	if(NULL == *ppTreeNode){
		*ppTreeNode = create_tree_node(data);
		assert(NULL != *ppTreeNode);
		(*ppTreeNode)->parent = pParent;
		return TRUE;
	}

	if(data < (*ppTreeNode)->data)
		return _insert_node_into_tree(&(*ppTreeNode)->left_child, data, *ppTreeNode);
	else
		return _insert_node_into_tree(&(*ppTreeNode)->right_child, data, *ppTreeNode);
}

STATUS insert_node_into_tree(TREE_NODE** ppTreeNode, int data)
{
	if(NULL == ppTreeNode)
		return FALSE;

	if(NULL == *ppTreeNode){
		*ppTreeNode = (TREE_NODE*)create_tree_node(data);
		assert(NULL != *ppTreeNode);
		return TRUE;
	}

	return _insert_node_into_tree(ppTreeNode, data, NULL);
}

static void test3()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 9));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 8));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(9 == pTreeNode->data);
	assert(8 == pTreeNode->left_child->data);
	assert(10 == pTreeNode->right_child->data);
	free(pTreeNode->left_child);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

/* judge parameter legality */
STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;
	return TRUE£»
}

static void test1()
{
	TREE_NODE* pTreeNode = NULL;
	assert(FALSE == delete_node_from_tree(NULL, 10));
	assert(FALSE == delete_node_from_tree(&pTreeNode, 10));
}


/* judge input data existance */
STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;

	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	return TRUE;
}

static void test2()
{
	TREE_NODE* pTreeNode = NULL;
	pTreeNode = create_tree_node(10);
	assert(FALSE == delete_node_from_tree(&pTreeNode, 11));
	free(pTreeNode);
}

/*
*               
*         10          ======>    NULL
*        /  \
*      NULL  NULL
*/

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;

	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}
		
		free(pTreeNode);
		return TRUE;
	}

	return TRUE;
}

static void test3()
{
	TREE_NODE* pTreeNode = NULL;
	pTreeNode = create_tree_node(10);
	assert(TRUE == delete_node_from_tree(&pTreeNode, 10));
	assert(NULL == pTreeNode);
}

/*
*               
*         10          ======>    5
*        /  \                  /  \
*      5  NULL                3    NULL
*     /                      
*    3
*/

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;

	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}
		
		free(pTreeNode);
		return TRUE;
	}

	return TRUE;
}

static void test4()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 5));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 3));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 10));
	assert(5 == pTreeNode->data);
	assert(NULL == pTreeNode->parent);
	free(pTreeNode->left_child);
	free(pTreeNode);
}

/*
*               
*         10          ======>    15
*        /  \                   /   \
*     NULL  15               NULL    20
*             \
*             20
*/

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;

	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
			*ppTreeNode = pTreeNode->right_child;
			pTreeNode->right_child->parent = NULL;
		}
		
		free(pTreeNode);
		return TRUE;
	}

	return TRUE;
}

static void test5()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 20));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 10));
	assert(15 == pTreeNode->data);
	assert(NULL == pTreeNode->parent);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

/*
*               
*         10          ======>     6
*        /  \                   /   \
*      6     15               5     15
*     /                      
*    5                         
*/

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	TREE_NODE* pLeftMax;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;
	
	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){
		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
			*ppTreeNode = pTreeNode->right_child;
			pTreeNode->right_child->parent = NULL;
		}else{
			pLeftMax = find_max_node(pTreeNode->left_child);
			if(pLeftMax == pTreeNode->left_child){
				*ppTreeNode = pTreeNode->left_child;
				(*ppTreeNode)->right_child = pTreeNode->right_child;
				(*ppTreeNode)->right_child->parent = *ppTreeNode;
				(*ppTreeNode)->parent = NULL;
			}
		}
		
		free(pTreeNode);
		return TRUE;
	}
	
	return TRUE;
}

static void test6()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 5));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 10));
	assert(6 == pTreeNode->data);
	assert(NULL == pTreeNode->parent);
	assert(15 == pTreeNode->right_child->data);
	assert(pTreeNode = pTreeNode->right_child->parent);
	assert(NULL == pTreeNode->parent);
	free(pTreeNode->left_child);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

/*
*               
*         10          ======>     8
*        /  \                   /   \
*      6     15               5     15
*       \                      
*        8                     
*/
STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	TREE_NODE* pLeftMax;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;
	
	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){
		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
			*ppTreeNode = pTreeNode->right_child;
			pTreeNode->right_child->parent = NULL;
		}else{
			pLeftMax = find_max_node(pTreeNode->left_child);
			if(pLeftMax == pTreeNode->left_child){
				*ppTreeNode = pTreeNode->left_child;
				(*ppTreeNode)->right_child = pTreeNode->right_child;
				(*ppTreeNode)->right_child->parent = *ppTreeNode;
				(*ppTreeNode)->parent = NULL;
			}else{
				pTreeNode->data = pLeftMax->data;
				pLeftMax->parent->right_child = NULL;
				pTreeNode = pLeftMax;
			}
		}
		
		free(pTreeNode);
		return TRUE;
	}
	
	return TRUE;
}

static void test7()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 8));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 10));
	assert(8 == pTreeNode->data);
	assert(NULL == pTreeNode->parent);
	assert(NULL == pTreeNode->left_child->right_child);
	assert(NULL == pTreeNode->parent);
	free(pTreeNode->left_child);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	TREE_NODE* pLeftMax;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;
	
	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){
		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
			*ppTreeNode = pTreeNode->right_child;
			pTreeNode->right_child->parent = NULL;
		}else{
			pLeftMax = find_max_node(pTreeNode->left_child);
			if(pLeftMax == pTreeNode->left_child){
				*ppTreeNode = pTreeNode->left_child;
				(*ppTreeNode)->right_child = pTreeNode->right_child;
				(*ppTreeNode)->right_child->parent = *ppTreeNode;
				(*ppTreeNode)->parent = NULL;
			}else{
				pTreeNode->data = pLeftMax->data;
				pLeftMax->parent->right_child = pLeftMax->left_child;
				pLeftMax->left_child->parent = pLeftMax->parent;
				pTreeNode = pLeftMax;
			}
		}
		
		free(pTreeNode);
		return TRUE;
	}
	
	return _delete_node_from_tree(pTreeNode);
}

/*
*               
*         10          ======>     10
*        /  \                      \
*      6     15                     15
*                                                         
*/

static void test8()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(6 == pTreeNode->left_child->data);
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 6));
	assert(NULL == pTreeNode->left_child);
	free(pTreeNode->right_child);
	free(pTreeNode);
}
/*
*               
*         10          ======>     10
*        /  \                    / 
*      6     15                 6   
*                                                         
*/

static void test9()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(15 == pTreeNode->right_child->data);
	assert(TRUE == delete_node_from_tree(&pTreeNode, 15));
	assert(NULL == pTreeNode->right_child);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

STATUS _delete_node_from_tree(TREE_NODE* pTreeNode)
{
	TREE_NODE* pLeftMax;
	
	if(NULL == pTreeNode-> left_child && NULL == pTreeNode->right_child){
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = NULL;
		else
			pTreeNode->parent->right_child = NULL;
	}
	
	free(pTreeNode);
	return TRUE;
}

/*
*               
*         10          ======>     10
*        /                      / 
*      6                      3   
*     /
*    3                                                        
*/

static void test10()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 3));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 6));
	assert(3 == pTreeNode->left_child->data);
	assert(pTreeNode = pTreeNode->left_child->parent);
	free(pTreeNode->left_child);
	free(pTreeNode);
}

/*
*               
*         10          ======>     10
*           \                       \
*           15                       12
*            /                    
*           12                                                 
*/
static void test11()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 12));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 15));
	assert(12 == pTreeNode->right_child->data);
	assert(pTreeNode = pTreeNode->right_child->parent);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

STATUS _delete_node_from_tree(TREE_NODE* pTreeNode)
{
	TREE_NODE* pLeftMax;
	
	if(NULL == pTreeNode-> left_child && NULL == pTreeNode->right_child){
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = NULL;
		else
			pTreeNode->parent->right_child = NULL;
	}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
		pTreeNode->left_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->left_child;
		else
			pTreeNode->parent->right_child = pTreeNode->left_child;
	}
	
	free(pTreeNode);
	return TRUE;
}

/*
*               
*         10          ======>    10
*        /                     / 
*      6                      8   
*       \
*        8                                                    
*/

static void test12()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 8));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 6));
	assert(8 == pTreeNode->left_child->data);
	assert(pTreeNode = pTreeNode->left_child->parent);
	free(pTreeNode->left_child);
	free(pTreeNode);
}

/*
*               
*        10          ======>    10
*          \                      \ 
*           15                     20 
*             \
*             20                                             
*/

static void test13()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 15));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 20));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 15));
	assert(20 == pTreeNode->right_child->data);
	assert(pTreeNode = pTreeNode->right_child->parent);
	free(pTreeNode->right_child);
	free(pTreeNode);
}

STATUS _delete_node_from_tree(TREE_NODE* pTreeNode)
{
	TREE_NODE* pLeftMax;
	
	if(NULL == pTreeNode-> left_child && NULL == pTreeNode->right_child){
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = NULL;
		else
			pTreeNode->parent->right_child = NULL;
	}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
		pTreeNode->left_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->left_child;
		else
			pTreeNode->parent->right_child = pTreeNode->left_child;
	}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
		pTreeNode->right_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->right_child;
		else
			pTreeNode->parent->right_child = pTreeNode->right_child;
	}
	
	free(pTreeNode);
	return TRUE;
}

/*
*               
*         10          ======>    10
*        /                     / 
*      6                      5    
*    /  \                      \
*   5    8                      8                              
*/

static void test14()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 5));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 8));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 6));
	assert(5 == pTreeNode->left_child->data);
	assert(pTreeNode = pTreeNode->left_child->parent);
	assert( 8 == pTreeNode->left_child->right_child->data);
	assert(pTreeNode->left_child = pTreeNode->left_child->right_child->parent);
	free(pTreeNode->left_child->right_child);
	free(pTreeNode->left_child);
	free(pTreeNode);
}

/*
*               
*         10          ======>    10
*        /                     / 
*       5                      4    
*      / \                    / \
*     2   6                  2   6
*      \                               
*       4
*/

static void test15()
{
	TREE_NODE* pTreeNode = NULL;
	assert(TRUE == insert_node_into_tree(&pTreeNode, 10));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 5));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 2));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 4));
	assert(TRUE == insert_node_into_tree(&pTreeNode, 6));
	assert(TRUE == delete_node_from_tree(&pTreeNode, 5));
	assert(4 == pTreeNode->left_child->data);
	assert(NULL == pTreeNode->left_child->left_child->right_child);
	free(pTreeNode->left_child->left_child);
	free(pTreeNode->left_child->right_child);
	free(pTreeNode->left_child);
	free(pTreeNode);
}

STATUS _delete_node_from_tree(TREE_NODE* pTreeNode)
{
	TREE_NODE* pLeftMax;
	
	if(NULL == pTreeNode-> left_child && NULL == pTreeNode->right_child){
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = NULL;
		else
			pTreeNode->parent->right_child = NULL;
	}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
		pTreeNode->left_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->left_child;
		else
			pTreeNode->parent->right_child = pTreeNode->left_child;
	}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
		pTreeNode->right_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->right_child;
		else
			pTreeNode->parent->right_child = pTreeNode->right_child;
	}else{
		pLeftMax = find_max_node(pTreeNode->left_child);
		if(pLeftMax == pTreeNode->left_child){
			
			if(pTreeNode == pTreeNode->parent->left_child)
				pTreeNode->parent->left_child = pTreeNode->left_child;
			else
				pTreeNode->parent->right_child = pTreeNode->left_child;
			
			pTreeNode->left_child->parent = pTreeNode->parent;
			pTreeNode->left_child->right_child = pTreeNode->right_child;
			pTreeNode->right_child->parent = pTreeNode-> left_child;
			
		}else{
			pTreeNode->data = pLeftMax->data;
			pLeftMax->parent->right_child = pLeftMax->left_child;
			pLeftMax->left_child->parent = pLeftMax->parent;
			pTreeNode = pLeftMax;
		}
	}
	
	free(pTreeNode);
	return TRUE;
}

STATUS _delete_node_from_tree(TREE_NODE* pTreeNode)
{
	TREE_NODE* pLeftMax;
	
	if(NULL == pTreeNode-> left_child && NULL == pTreeNode->right_child){
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = NULL;
		else
			pTreeNode->parent->right_child = NULL;
	}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
		pTreeNode->left_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->left_child;
		else
			pTreeNode->parent->right_child = pTreeNode->left_child;
	}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
		pTreeNode->right_child->parent = pTreeNode->parent;
		
		if(pTreeNode == pTreeNode->parent->left_child)
			pTreeNode->parent->left_child = pTreeNode->right_child;
		else
			pTreeNode->parent->right_child = pTreeNode->right_child;
	}else{
		pLeftMax = find_max_node(pTreeNode->left_child);
		if(pLeftMax == pTreeNode->left_child){
			
			if(pTreeNode == pTreeNode->parent->left_child)
				pTreeNode->parent->left_child = pTreeNode->left_child;
			else
				pTreeNode->parent->right_child = pTreeNode->left_child;
			
			pTreeNode->left_child->parent = pTreeNode->parent;
			pTreeNode->left_child->right_child = pTreeNode->right_child;
			pTreeNode->right_child->parent = pTreeNode-> left_child;
			
		}else{
			pTreeNode->data = pLeftMax->data;
			pLeftMax->parent->right_child = pLeftMax->left_child;
			pLeftMax->left_child->parent = pLeftMax->parent;			
			pTreeNode = pLeftMax;
		}
	}
	
	free(pTreeNode);
	return TRUE;
}

STATUS delete_node_from_tree(TREE_NODE** ppTreeNode, int data)
{
	TREE_NODE* pTreeNode;
	TREE_NODE* pLeftMax;
	
	if(NULL == ppTreeNode || NULL == *ppTreeNode)
		return FALSE;
	
	pTreeNode = find_data_in_tree_node(*ppTreeNode, data);
	if(NULL == pTreeNode)
		return FALSE;
	
	if(*ppTreeNode == pTreeNode){
		
		if(NULL == pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = NULL;
		}else if(NULL != pTreeNode->left_child && NULL == pTreeNode->right_child){
			*ppTreeNode = pTreeNode->left_child;
			pTreeNode->left_child->parent = NULL;
		}else if(NULL == pTreeNode->left_child && NULL != pTreeNode->right_child){
			*ppTreeNode = pTreeNode->right_child;
			pTreeNode->right_child->parent = NULL;
		}else{
			pLeftMax = find_max_node(pTreeNode->left_child);
			if(pLeftMax == pTreeNode->left_child){
				*ppTreeNode = pTreeNode->left_child;
				(*ppTreeNode)->right_child = pTreeNode->right_child;
				(*ppTreeNode)->right_child->parent = *ppTreeNode;
				(*ppTreeNode)->parent = NULL;
			}else{
				pTreeNode->data = pLeftMax->data;
				pLeftMax->parent->right_child = pLeftMax->left_child;
				pLeftMax->left_child->parent = pLeftMax->parent;
				pTreeNode = pLeftMax;
			}
		}
		
		free(pTreeNode);
		return TRUE;
	}
	
	return _delete_node_from_tree(pTreeNode);
}

/*
sequence binary tree
*/
typedef struct _TREE_NODE
{
	int data;
	int number;
	struct _TREE_NODE* left_child;
	struct _TREE_NODE* right_child;
}TREE_NODE;

STATUS _insert_node_into_tree(TREE_NODE* pTreeNode, int data)
{
	TREE_NODE* pNode;

	while(1){
		if(data < pTreeNode->data){
			if(NULL == pTreeNode->left_child){
				pNode = create_tree_node(data);
				assert(NULL != pNode);
				pNode->number = pTreeNode->number << 1;
				pTreeNode->left_child = pNode;
				break;
			}else
				pTreeNode = pTreeNode->left_child;
		}else{
			if(NULL == pTreeNode->right_child){
				pNode = create_tree_node(data);
				assert(NULL != pNode);
				pNode->number = pTreeNode->number << 1 + 1;
				pTreeNode->right_child = pNode;
				break;
			}else
				pTreeNode = pTreeNode->right_child;
		}
	}

	return TRUE;
}

STATUS insert_node_into_tree(TREE_NODE** ppTreeNode, int data)
{
	if(NULL == ppTreeNode)
		return FALSE;
	
	if(NULL == *ppTreeNode){
		*ppTreeNode = (TREE_NODE*)create_tree_node(data);
		assert(NULL != *ppTreeNode);
		(*ppTreeNode)->number = 1;
		return TRUE;
	}
	
	return _insert_node_into_tree(*ppTreeNode, data);
}

typedef struct _DATA
{
	int data;
	int number;
}DATA;
