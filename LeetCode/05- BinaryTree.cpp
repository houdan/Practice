#include "Common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;

namespace
{
	// input array needs to be pre-order or level-order
	TreeNode* buildBST(int a[], int size)
	{
		TreeNode* root = (size > 0) ? new TreeNode(a[0]) : NULL;
		for(int i=1; i<size; i++)
		{
			TreeNode* curr = root;
			while(curr)
			{
				if(a[i] < curr->val)
					curr = curr->left;
				else if (a[i] > curr->val)
					curr = curr->right;
				else
					std::abort(); // !!!!!!! no duplicate values!!
			}
			curr = new TreeNode(a[i]);
		}
		return root;
	}

	// post-order
	void deleteBinaryTree(TreeNode *root)
	{
		if(!root) return;

		deleteBinaryTree(root->left);
		deleteBinaryTree(root->right);
		delete root;
	}
}

// pre-order
bool isSameTree(TreeNode *p, TreeNode *q) 
{
	if(p == NULL && q == NULL)
		return true;

	if(!(p && q) || p->val != q->val)
		return false;

	return isSameTree(p->left, q->left) &&
		   isSameTree(p->right, q->right);
}

void test_isSameTree()
{
	// Passed all tests on OJ
}

struct IsValidBSTWithPreOrder
{
	static bool isBST(TreeNode *node, int min, int max)
	{
		if(!node) return true; // base case

		if(node->val <= min || node->val >= max) // !!!!!!! no duplicate values!!
			return false; 

		return isBST(node->left, min, node->val) &&
			   isBST(node->right, node->val, max);
	}

	static bool run(TreeNode *root)
	{
		return isBST(root, INT_MIN, INT_MAX);
	}
};

struct IsValidBSTWithInOrder
{
	static bool isBST(TreeNode *node, int &prev)
	{
		if(!node) return true; // base case

		if(!isBST(node->left, prev) || node->val <= prev)  // !!!!!!! no duplicate values!!
			return false;

		prev = node->val;
		return isBST(node->right, prev);
	}

	static bool run(TreeNode *root)
	{
		int prev = INT_MIN;
		return isBST(root, prev);
	}
};

template<class Func>
void test_isValidBST()
{
	const int count = 10;

	// complete binary tree
	TreeNode nodes[count];
	for(int i=0; i<count; i++)
	{
		nodes[i].left = (2*i+1<count) ? &nodes[2*i+1] : NULL;
		nodes[i].right = (2*i+2<count) ? &nodes[2*i+2] : NULL;
	}
	
	for(int i=0; i<count; i++)
		nodes[i].val = i;

	assert(!Func::run(&nodes[0]));

	int a[count] = {6,3,8,1,5,7,9,0,2,4};
	for(int i=0; i<count; i++)
		nodes[i].val = a[i];

	assert(Func::run(&nodes[0]));

	int b[count] = {6,3,8,1,5,7,9,0,1,4};
	for(int i=0; i<count; i++)
		nodes[i].val = b[i];

	assert(!Func::run(&nodes[0]));
}

// pre-order traversal
void serilaizeBST(TreeNode *root, stringstream &out)
{
	if(!root) return;

	out << " " << root->val;  // !!! need white space since using stringstream 
	serilaizeBST(root->left, out);
	serilaizeBST(root->right, out);
}

void test_serilaizeBST()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root_a = buildBST(a, count);

	stringstream ss;
	serilaizeBST(root_a, ss);

	int index = 0;
	while(!ss.eof())
	{
		int val;
		ss >> val;
		assert(val == a[index++]);
	}
}

TreeNode* deserializeBSTHelper(stringstream &in, int min, int max, int &val)
{
	if(val > min && val < max)
	{
		int cur_val = val;
		TreeNode* node = new TreeNode(cur_val);

		if(!in.eof())
		{
			in >> val;
			node->left = deserializeBSTHelper(in, min, cur_val, val);
			node->right = deserializeBSTHelper(in, cur_val, max, val);
		}
		return node;
	}	
	else
		return NULL;
}

TreeNode* deserializeBST(stringstream &in)
{
	if(!in.eof())
	{
		int val;
		in >> val;
		return deserializeBSTHelper(in, INT_MIN, INT_MAX, val);
	}
	else
		return NULL;
}

void test_deserializeBST()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root_a = buildBST(a, count);

	stringstream ss;
	serilaizeBST(root_a, ss);

	TreeNode* root_b = deserializeBST(ss);
	assert(isSameTree(root_a, root_b));

	deleteBinaryTree(root_a);
	deleteBinaryTree(root_b);
}

TreeNode* sortedArrayToBST(vector<int> &a, int lo, int hi) 
{
	if(lo > hi) return NULL; // base case

	int mi = (lo + hi) / 2;

	TreeNode* node = new TreeNode(a[mi]);
	node->left = sortedArrayToBST(a, lo, mi-1);
	node->right = sortedArrayToBST(a, mi+1, hi);

	return node;
}

TreeNode* sortedArrayToBST(vector<int> &a) 
{
	return sortedArrayToBST(a, 0, a.size()-1);
}

void test_sortedArrayToBST()
{
	// Passed all tests on OJ
}

TreeNode *sortedListToBST(ListNode *&head, int lo, int hi) 
{
	if(lo > hi) return NULL; 

	int mi = lo + (hi-lo)/2; // avoid overflow
	TreeNode *left = sortedListToBST(head, lo, mi-1);
	TreeNode *parent = new TreeNode(head->val); // in-order
	parent->left = left;
	head = head->next; // traverse list
	parent->right = sortedListToBST(head, mi+1, hi);

	return parent;
}

TreeNode *sortedListToBST(ListNode *head) 
{
	int n = 0;
	ListNode *curr = head;
	while(curr)
	{
		n++;
		curr = curr->next;
	}
	return sortedListToBST(head, 0, n-1);
}

void test_sortedListToBST()
{
	// Passed all tests on OJ
}

void test_binaryTree()
{
	// Determine if two binary trees are same
	test_isSameTree();

	// Determine if a Binary Tree is a BST 
	test_isValidBST<IsValidBSTWithPreOrder>();  // check range
	test_isValidBST<IsValidBSTWithInOrder>();   // check order

	// Serialize BST
	test_serilaizeBST();  // pre-order, maybe level-order?

	// Deserialize BST
	test_deserializeBST(); // pre-order, maybe level-order?

	// Convert Sorted Array to Balanced BST
	test_sortedArrayToBST();

	// Convert Sorted List to Balanced BST ????
	test_sortedListToBST();
}