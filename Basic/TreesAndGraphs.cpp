#include "Common.h"
#include <assert.h>

using namespace std;

template<class T>
struct BinaryTreeInOrderRecursion
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		if(!root)
			return;

		run(root->left, out);
		out.enqueue(root->data);
		run(root->right, out);
	}
};

template<class T>
struct BinaryTreeInOrderIterative
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		Stack<NodeBT<T>*> stack; // record each parent node

		while(root || !stack.empty()) // !!!!!!!!!!!!!!!!!!!!!
		{
			if(root)
			{
				stack.push(root);
				root = root->left;
			}
			else
			{
				root = stack.pop();
				out.enqueue(root->data);
				root = root->right;
			}
		}
	}
};

template<class T>
struct BinaryTreePreOrderRecursion
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		if(!root)
			return;

		out.enqueue(root->data);
		run(root->left, out);
		run(root->right, out);
	}
};


template<class T>
struct BinaryTreePreOrderIterative
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		Stack<NodeBT<T>*> stack;
		
		while(root || !stack.empty())
		{
			if(root)
			{
				out.enqueue(root->data);
				stack.push(root);
				root = root->left;
			}
			else // when it goes beyond leaf
			{
				root = stack.pop();
				root = root->right;
			}
		}
	}
};

template<class T>
struct BinaryTreePostOrderRecursion
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		if(!root)
			return;

		run(root->left, out);
		run(root->right, out);
		out.enqueue(root->data);
	}
};

template<class T>
struct BinaryTreePostOrderIterative
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		Stack<NodeBT<T>*> stack;
		/*while(root || !stack.empty())
		{
			if(root)
			{
				stack.push(root);
				root = root->left;
			}
			else
			{
				root = stack.pop();
				root = root->right;
			}
		}*/
	}
};

template<class T>
struct BinaryTreeLevelOrder
{
	static void run(NodeBT<T> *root, Queue<T> &out)
	{
		Queue<NodeBT<T>*> queue;

		while(root) // !!!! better than condition on queue.empty(), since it also checks input root is NULL case
		{
			out.enqueue(root->data);

			if(root->left)
				queue.enqueue(root->left);

			if(root->right)
				queue.enqueue(root->right);

			if(queue.empty())
				break;
			root = queue.dequeue();
		}
	}
};

enum Traversal
{
	InOrder,
	PreOrder,
	PostOrder,
	LevelOrder,

	NumTypes
};

template<template<class T>class Func>
void test_binaryTreeTraversal(Traversal type)
{
	// Complete Binary Tree
	const int count = 10;
	NodeBT<int> nodes[count];
	for(int i=0; i<count; i++)
	{
		nodes[i].data = i;
		nodes[i].left = (2*i+1<count) ? &nodes[2*i+1] : NULL;
		nodes[i].right = (2*i+2<count) ? &nodes[2*i+2] : NULL;
	}

	int order[Traversal::NumTypes][count] =
	{
		{7,3,8,1,9,4,0,5,2,6}, // InOrder
		{0,1,3,7,8,4,9,2,5,6}, // PreOrder
		{7,8,3,9,4,1,5,6,2,0}, // PostOrder
		{0,1,2,3,4,5,6,7,8,9}, // LevelOrder
	};

	Queue<int> out;
	Func<int>::run(&nodes[0], out);

	for(int i=0; i<count; i++)
	{
		int data = out.dequeue();
		assert(data == order[type][i]);
	}
	assert(out.empty());
}

void test_treesAndGraphs()
{
	test_binaryTreeTraversal<BinaryTreeInOrderRecursion>(InOrder);
	test_binaryTreeTraversal<BinaryTreeInOrderIterative>(InOrder);

	test_binaryTreeTraversal<BinaryTreePreOrderRecursion>(PreOrder);
	test_binaryTreeTraversal<BinaryTreePreOrderIterative>(PreOrder);

	test_binaryTreeTraversal<BinaryTreePostOrderRecursion>(PostOrder);
	//test_binaryTreeTraversal<BinaryTreePostOrderIterative>(PostOrder); // implement this
	
	test_binaryTreeTraversal<BinaryTreeLevelOrder>(LevelOrder);
}