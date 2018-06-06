#include "cctree.h"

STATUS TreeCreate(CC_TREE **Tree)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	CC_PTREE ptr = (CC_PTREE)malloc(sizeof(CC_TREE));

	if (NULL == ptr)
	{
		return STATUS_UNSUCCESSFUL;
	}

	ptr->left = NULL;
	ptr->right = NULL;

	ptr->size = 0;

	*Tree = ptr;

	return STATUS_SUCCESS;
}

int TreeDestroy(CC_TREE **Tree)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if ((*Tree)->left != NULL)
	{
		TreeDestroy(&(*Tree)->left);
	}

	if ((*Tree)->right != NULL)
	{
		TreeDestroy(&(*Tree)->right);
	}

	free(*Tree);

	return STATUS_SUCCESS;
}

static CC_PTREE insert(CC_TREE *root, int v) {
	if (root == NULL) 
	{
		root = (CC_PTREE)malloc(sizeof(CC_TREE));

		if (NULL == root)
		{
			return NULL;
		}

		root->value = v;
		root->left = NULL;
		root->right = NULL;
	}
	else if (v < root->value) 
	{
		root->left = insert(root->left, v);
	}
	else 
	{
		root->right = insert(root->right, v);
	}

	return root;
}

STATUS TreeInsert(CC_TREE *Tree, int Value)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Tree->size == 0)
	{
		Tree->value = Value;
	}
	else if (Value < Tree->value)
	{
		Tree->left = insert(Tree->left, Value);
	}
	else
	{
		Tree->right = insert(Tree->right, Value);
	}

	Tree->size++;
	return STATUS_SUCCESS;
}

CC_PTREE minValueNode(CC_PTREE node)
{
	CC_PTREE current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

CC_PTREE deleteNode(CC_PTREE root, int Value)
{
	if (root == NULL) return root;

	if (Value < root->value)
		root->left = deleteNode(root->left, Value);

	else if (Value > root->value)
		root->right = deleteNode(root->right, Value);

	else
	{
		if (root->left == NULL)
		{
			 CC_PTREE temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			CC_PTREE temp = root->left;
			free(root);
			return temp;
		}

		CC_PTREE temp = minValueNode(root->right);

		root->value = temp->value;

		root->right = deleteNode(root->right, temp->value);
	}

	return root;
}

// Removes all elements equal to Value
STATUS TreeRemove(CC_TREE *Tree, int Value)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	Tree = deleteNode(Tree, Value);

	return STATUS_SUCCESS;
}

/*
*  Return:
*      1   - Tree contains value
*      0   - Tree does not contain Value
*/
int TreeContains(CC_TREE *Tree, int Value)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Tree->value == Value)
	{
		return 1;
	}

	if (Value < Tree->value)
	{
		return TreeContains(Tree->left, Value);
	}
	else
	{
		return TreeContains(Tree->right, Value);
	}

	return 0;
}

int TreeGetCount(CC_TREE *Tree) {
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	return Tree->size;
}

int TreeGetHeight(CC_TREE *Tree)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}
	else
	{
		int lDepth = TreeGetHeight(Tree->left);
		int rDepth = TreeGetHeight(Tree->right);

		if (lDepth > rDepth)
			return(lDepth + 1);
		else return(rDepth + 1);
	}
}


static STATUS deleteNodes(CC_PTREE node)
{
	if (NULL == node)
	{
		return 0;
	}

	deleteNodes(node->left);
	deleteNodes(node->right);

	free(node);
	return 0;
}


STATUS TreeClear(CC_TREE *Tree)
{
	if (NULL == Tree)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	deleteNodes(Tree->left);
	deleteNodes(Tree->right);

	return STATUS_SUCCESS;
}

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
	if (NULL == Tree || NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Index == 0)
	{
		*Value = Tree->value;
	}

	if (Tree->left)
		TreeGetNthPreorder(Tree->left, Index - 1, Value);

	if (Tree->right)
		TreeGetNthPreorder(Tree->right, Index-1, Value);

	return STATUS_SUCCESS;
}
int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
	if (NULL == Tree || NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Tree->left) 
		TreeGetNthInorder(Tree->left, Index - 1, Value);

	if (Index == 0)
	{
		*Value = Tree->value;
	}

	if (Tree->right)
		TreeGetNthInorder(Tree->right, Index - 1, Value);

	return STATUS_SUCCESS;
}

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
	if (NULL == Tree || NULL == Value)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (Tree->left)
		TreeGetNthPostorder(Tree->left, Index - 1, Value);

	if (Tree->right)
		TreeGetNthPostorder(Tree->right, Index - 1, Value);

	if (Index == 0)
	{
		*Value = Tree->value;
	}

	return STATUS_SUCCESS;
}

int main()
{
	CC_PTREE tree;
	TreeCreate(&tree);

	TreeInsert(tree, 7);
	TreeInsert(tree, 8);
	TreeInsert(tree, 4);
	TreeInsert(tree, 2);
	TreeInsert(tree, 9);
	TreeInsert(tree, 1);
	TreeInsert(tree, 4);

	int c;
	c=TreeContains(tree, 0);
	c=TreeContains(tree, 2);

	c=TreeGetCount(tree);

	int value;
	TreeGetNthPostorder(tree, 3, &value);

	TreeDestroy(tree);

	return 0;
}