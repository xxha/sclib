/*
The advantage of balance binary tree is sorting. but it's not good for find.
to avoid this disadvantage, we create following data structure:
	1. hash binary tree.
	2. avl tree
	3. rb tree
*/

/*
Hash binary tree
By adding a next pointer to binary tree, and build a hash table, 
then we can use hash table to find data, other than balance binary tree find. 
*/

typedef struct _HASH_TREE
{
	int data;
	struct _HASH_TREE* next;
	struct _HASH_TREE* left;
	struct _HASH_TREE* right;
}HASH_TREE;

STATUS add_node_into_tree(HASH_TREE** ppHash, int data)
{

	/* add hash node into tree */

	/* add hash node into hash table */

	return TRUE;
}

STATUS delete_node_from_tree(HASH_TREE** ppHash, int data)
{
	HASH_TREE* pNode;
	/* delete hash node from tree, but not free space*/
	
	/* delete hash node from hash table */
	
	free(pNode);
	return TRUE;
}
