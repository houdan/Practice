#include "Common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <unordered_map>
#include <stack>

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
			while(true) // traverse down the BST
			{
				if(a[i] < curr->val)
				{
					if(!curr->left) 
					{
						curr->left = new TreeNode(a[i]);
						break;
					}
					else
						curr = curr->left;
				}
				else if (a[i] > curr->val)
				{
					if(!curr->right) 
					{
						curr->right = new TreeNode(a[i]);
						break;
					}
					else
						curr = curr->right;
				}
				else
					std::abort(); // !!!!!!! no duplicate values!!
			}
			
		}
		return root;
	}

	TreeNode* findNode(TreeNode *root, int val)
	{
		if(!root) return NULL;

		if(root->val == val)
			return root;

		TreeNode* node = findNode(root->left, val);
		if(!node) node = findNode(root->right, val);
		return node;
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

bool isBalancedHelper(TreeNode *p, int &height)
{
	if(!p) // base case
	{
		height = -1; // leaf node has height 0
		return true;
	}

	int left_height, right_height;
	if(!isBalancedHelper(p->left, left_height) ||
	   !isBalancedHelper(p->right, right_height))
		return false;

	height = std::max(left_height, right_height) + 1;
	return abs(left_height - right_height) <= 1;
}

bool isBalanced(TreeNode *p)
{
	int height;
	return isBalancedHelper(p, height);
}

void test_isBalanced()
{
	// passed all tests on OJ
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
void serializeBST(TreeNode *root, stringstream &out)
{
	if(!root) return;

	out << " " << root->val;  // !!! need white space since using stringstream 
	serializeBST(root->left, out);
	serializeBST(root->right, out);
}

void test_serializeBST()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root_a = buildBST(a, count);

	stringstream ss;
	serializeBST(root_a, ss);

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
		TreeNode* node = new TreeNode(cur_val); // !!!!!!!! only create node when the val passed in satisfies the range !!!

		if(!in.eof())  // !!!!!!! could use if(in >> val) instead
		{
			in >> val;  // !!!!!!!!! must read value to pass into recursion, because same value might be checked against several ranges
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
	serializeBST(root_a, ss);

	TreeNode* root_b = deserializeBST(ss);
	assert(isSameTree(root_a, root_b));

	deleteBinaryTree(root_a);
	deleteBinaryTree(root_b);
}

// pre-order or level-order
void serilaizeBT(TreeNode *root, stringstream &out)
{
	if(root) 
	{
		out << " " << root->val;
		serilaizeBT(root->left, out);
		serilaizeBT(root->right, out);
	}
	else
		out << " #";
}

void test_serializeBT()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root_a = buildBST(a, count);

	stringstream ss;
	serilaizeBT(root_a, ss);

	char *b = " 6 3 1 0 # # 2 # # 5 4 # # # 8 7 # # 9 # #";
	string s = ss.str();
	assert(s.compare(b) == 0);

	deleteBinaryTree(root_a);
}

TreeNode* deserializeBT(stringstream &in)
{
	TreeNode* node = NULL;
	if(!in.eof())
	{
		// !!!!!!!! better to wrap the check logics in to readNextToken() helper function
		char c = in.peek();
		if(c == ' ') in.read(&c, 1); // skip while space

		c = in.peek();
		if(c == '#') in.read(&c, 1); // skip sentinal
		else
		{
			int val;
			in >> val;
			node = new TreeNode(val);
			node->left = deserializeBT(in);
			node->right = deserializeBT(in);
		}	
	}
	return node;
}

void test_deserializeBT()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root_a = buildBST(a, count);

	stringstream ss;
	serilaizeBT(root_a, ss);

	TreeNode* root_b = deserializeBT(ss);
	assert(isSameTree(root_a, root_b));

	deleteBinaryTree(root_a);
	deleteBinaryTree(root_b);
}

TreeNode* buildBTFromInAndPreOrder(int preorder[], int inorder[], int n, int offset, const unordered_map<int, int> &inorderMap)
{
	if(n == 0) return NULL;

	int val = preorder[0];
	unordered_map<int, int>::const_iterator it = inorderMap.find(val);
	assert(it != inorderMap.end());
	int index = it->second - offset;  //!!!!!!!!!! offset from original in-order array

	// preorder [root | left-subtree | right-subtee]
	// inorder  [left-subtree | root | right-subtee]
	TreeNode* root = new TreeNode(val);
	root->left = buildBTFromInAndPreOrder(preorder+1, inorder, index, offset, inorderMap);
	root->right = buildBTFromInAndPreOrder(preorder+1+index, inorder+1+index, n-index-1, offset+index+1, inorderMap);
	return root;
}

TreeNode* buildBTFromInAndPreOrder(vector<int> &preorder, vector<int> &inorder)
{
	unordered_map<int, int> inorderMap;
	for(int i=0; i<inorder.size(); i++)
		inorderMap[inorder[i]] = i;
	return buildBTFromInAndPreOrder(preorder.data(), inorder.data(), inorder.size(), 0, inorderMap);
}

void test_buildBTFromInAndPreOrder()
{
	// passed all tests on OJ
}

TreeNode* buildBTFromInAndPostOrder()
{
	return NULL;
}

void test_buildBTFromInAndPostOrder()
{

}

// build while pre-order traversal
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

// 
TreeNode *findLCA(TreeNode *root, TreeNode *p, TreeNode *q) 
{
	if(!root) return NULL; 

	// stop search root's subtrees if root is one of either nodes.
	// if we can't find the other node when searching other part of the tree, 
	// then it must be in root's subtrees so that the root is LCA 
	if(root == p || root == q)
		return root; 

	TreeNode* left = findLCA(root->left, p, q);

	// optimization: we could stop search right subtree, if left subtree returns 
	// a non-null node that is not either p or q, so it must be LCA already
	if(left && left != p && left != q)
		return left;

	TreeNode* right = findLCA(root->right, p, q);

	// if both subtree returns non-null node, each of them must be either p or q,
	// so the root is LCA
	if(left && right)
		return root;

	// either only one subtree has either p or q, or both subtrees don't have them
	return left ? left : right;
}

void test_findLCA()
{
	const int count = 10;
	int a[count] = {6,3,1,0,2,5,4,8,7,9}; // pre-order
	TreeNode* root = buildBST(a, count);

	{
		TreeNode* p = findNode(root, 0);
		TreeNode* q = findNode(root, 4);
		TreeNode* r = findNode(root, 3);
		assert(findLCA(root, p, q) == r);
	}

	{
		TreeNode* p = findNode(root, 3);
		TreeNode* q = findNode(root, 2);
		assert(findLCA(root, p, q) == p);
	}

	{
		TreeNode* p = findNode(root, 2);
		TreeNode* q = findNode(root, 9);
		TreeNode* r = findNode(root, 6);
		assert(findLCA(root, p, q) == r);
	}
}

void sumNumbers(TreeNode *root, int num, int &sum)
{
	if(!root) return; // !!!!!! failure case

	num = num * 10 + root->val;

	if(!root->left && !root->right) // !!!!!!!! base case: add to sum at leaf !!!!!
	{
		sum += num;
		return;
	}

	sumNumbers(root->left, num, sum);
	sumNumbers(root->right, num, sum);
}

int sumNumbers(TreeNode *root) 
{
	int num = 0, sum = 0;
	sumNumbers(root, num, sum);
	return sum;
}

void test_sumNumbers()
{
	// passed all tests on OJ
}

void maxPathSum(TreeNode *root, int &maxSum, int &maxSumToRoot) 
{
	if(!root)
	{
		maxSum = INT_MIN;  // !!!!!!!!!! can't be 0
		maxSumToRoot = INT_MIN;
		return;
	}

	// visit subtrees
	int maxSumLeft, maxSumToRootLeft;
	int maxSumRight, maxSumToRootRight;
	maxPathSum(root->left, maxSumLeft, maxSumToRootLeft);
	maxPathSum(root->right, maxSumRight, maxSumToRootRight);

	// update maxSumToRoot
	maxSumToRoot = root->val + std::max(0, std::max(maxSumToRootLeft, maxSumToRootRight)); // max sum connecting to root
		
	// update maxSum
	maxSum = std::max(maxSumLeft, maxSumRight);  // max sum in each subtree
	maxSum = std::max(maxSum, root->val + std::max(0, maxSumToRootLeft) + std::max(0, maxSumToRootRight)); // !!!!!!!!! max sum passing through root
}

int maxPathSum(TreeNode *root) 
{
	int maxSum, maxSumToRoot;
	maxPathSum(root, maxSum, maxSumToRoot);
	return maxSum;
}

void test_maxPathSum()
{
	stringstream s;
	s << " -2 1";

	TreeNode *root = deserializeBT(s);

	assert(1 == maxPathSum(root));

	deleteBinaryTree(root);
}

// iterative solution
vector<int> inorderTraversal(TreeNode *root) 
{
	stack<TreeNode*> stack;
	vector<int> output;

	while(root || !stack.empty()) // !!!!!!!!!!!!!!!
	{
		if(root)
		{
			stack.push(root);
			root = root->left; // visit left-subtree
		}
		else
		{
			root = stack.top();
			stack.pop(); // !!!!!!!!!!!

			output.push_back(root->val); // visit root
			root = root->right;  // visit right-subtree
		}
	}

	return output; // !!!!!!!!!! return
}

void test_inorderTraversal()
{
	// all tests passed on OJ
}

vector<int> inorderTraversalWithoutStack(TreeNode2 *root)
{
	TreeNode2 *curr = root;
	TreeNode2 *last = NULL;

	vector<int> output;

	while(curr)
	{
		if(curr->parent == last) // traverse down
		{
			if(curr->left)
			{
				last = curr;
				curr = curr->left; // traverse left subtree
				continue; 
			}
			else
				last = NULL; // !!!!!!! 
		}

		if(last == curr->left) // traverse up from left subtree
		{
			output.push_back(curr->val);

			if(curr->right)
			{
				last = curr;
				curr = curr->right; // traverse right subtree
				continue;
			}
			else
				last = NULL; // !!!! 
		}

		if(last == curr->right) // traverse up from right subtree
		{
			last = curr;
			curr = curr->parent;
		}
	}

	return output; // !!!!!!!!!
}

void test_inorderTraversalWithoutStack()
{
	const int count = 10;

	// complete binary tree
	TreeNode2 nodes[count];
	for(int i=0; i<count; i++)
	{
		nodes[i].val = i;
		nodes[i].left = (2*i+1<count) ? &nodes[2*i+1] : NULL;
		nodes[i].right = (2*i+2<count) ? &nodes[2*i+2] : NULL;
		nodes[i].parent = (i>0) ? &nodes[(i-1)/2] : NULL;
	}

	int a[count] = {7,3,8,1,9,4,0,5,2,6};
	vector<int> output = inorderTraversalWithoutStack(&nodes[0]);
	for(int i=0; i<count; i++)
		assert(a[i] == output[i]);
}

// in-order, return rightmost node in the tree
void findWrongNode(TreeNode *root, TreeNode *&n1, TreeNode *&n2, TreeNode *&prev)
{
	if(!root)
		return;

	findWrongNode(root->left, n1, n2, prev);

	if(prev && prev->val > root->val)
	{
		if(!n1) 
			n1 = prev;
		n2 = root; // !!!!!!!!! n2 might be immediately after n1, so need to always set
	}

	prev = root; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	findWrongNode(root->right, n1, n2, prev);
}

void recoverTree(TreeNode *root) 
{
	TreeNode *n1 = NULL;
	TreeNode *n2 = NULL;
	TreeNode *prev = NULL;
	findWrongNode(root, n1, n2, prev);

	assert(n1 && n2);
	std::swap(n1->val, n2->val);
}

void test_recoverTree()
{
	// all tests passed on OJ
}

vector<vector<int> > zigzagLevelOrder(TreeNode *root) 
{
	vector<vector<int> > result;
	if(!root) return result;

	stack<TreeNode*> curr, next;
	curr.push(root);

	result.resize(1);
	int curr_level = 0;
	bool curr_left_to_right = true;
	
	while(!curr.empty())
	{
		TreeNode *node = curr.top();
		result[curr_level].push_back(node->val);

		if(curr_left_to_right)
		{
			if(node->left) next.push(node->left);
			if(node->right) next.push(node->right);
		}
		else
		{
			if(node->right) next.push(node->right);
			if(node->left) next.push(node->left);
		}

		curr.pop();
		if(curr.empty() && !next.empty())
		{
			result.resize(result.size()+1);
			curr_level += 1;
			curr_left_to_right = !curr_left_to_right;  // !!!!!!!!!! change direction
			curr.swap(next);
		}
	}
}

void test_zigzagLevelOrder()
{
	zigzagLevelOrder(NULL);
}

void connect(TreeLinkNode *root) 
{
	TreeLinkNode *head = root;

	while(head)
	{
		if(!head->left) break; // reached last level

		TreeLinkNode *curr = head;
		while(curr)
		{
			curr->left->next = curr->right;
			curr->right->next = (curr->next) ? curr->next->left : NULL;
			curr = curr->next;
		}

		head = head->left;
	}
}

void test_connect()
{
	// all tests passed on OJ
}

void test_binaryTree()
{
	// Determine if two binary trees are same
	test_isSameTree();

	// Determine if a Binary Tree is a BST 
	test_isValidBST<IsValidBSTWithPreOrder>();  // check range
	test_isValidBST<IsValidBSTWithInOrder>();   // check order

	// Recover Binary Search Tree (two nodes swapped)
	test_recoverTree();

	// Serialize BST
	test_serializeBST();  // pre-order

	// Deserialize BST
	test_deserializeBST(); // pre-order

	// Serialize BT
	test_serializeBT();  // pre-order or level-order

	// Deserialize BT
	test_deserializeBT(); // pre-order or level-order

	// Construct BT from in-order and pre-order Traversal
	test_buildBTFromInAndPreOrder();

	// Construct BT from in-order and post-order Traversal
	//test_buildBTFromInAndPostOrder();

	// Convert Sorted Array to Balanced BST
	test_sortedArrayToBST();

	// Convert Sorted List to Balanced BST ????
	test_sortedListToBST();

	// Lowest Common Ancestor of a Binary Tree
	test_findLCA();

	// Sum Root to Leaf Numbers
	test_sumNumbers();

	// Maximum Path Sum (start and end at any node)
	test_maxPathSum();

	// Binary Tree Inorder Traversal
	test_inorderTraversal();

	// Binary Tree Inorder Traversal without stack
	test_inorderTraversalWithoutStack();

    // Binary Tree Zigzag Level Order Traversal 
	test_zigzagLevelOrder();

	// Populating Next Right Pointers in Each Node
	test_connect();
}