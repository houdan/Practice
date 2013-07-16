#include "Common.h"
#include <unordered_set>
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

struct TraversalBT
{
	enum Type
	{
		InOrder,
		PreOrder,
		PostOrder,
		LevelOrder,

		NumTypes
	};
};

template<template<class T>class Func>
void test_binaryTreeTraversal(TraversalBT::Type type)
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

	int order[TraversalBT::NumTypes][count] =
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


template<class T>
struct GraphBreadthFirst
{
	static void run(NodeG<T> *root, Queue<T> &out)
	{
		if(!root) return;

		Queue<NodeG<T>*> q; // visited, not yet explored
		unordered_set<NodeG<T>*> visited;

		out.enqueue(root->data);
		visited.insert(root);
		q.enqueue(root);
		
		while(!q.empty())
		{
			List<NodeG<T>*>::Iterator it(q.dequeue()->neighbors);
			for(; it.good(); it.advance())
			{
				NodeG<T> *node = it.value();
				if(visited.count(node)>0)
					continue;

				out.enqueue(node->data); // visit 
				visited.insert(node); // mark as visited
				q.enqueue(node); // enqueue to explore later
			}
		}
	}
};

template<class T>
struct GraphDepthFirstRecursion
{
	static void run(NodeG<T> *root, Queue<T> &out)
	{
		
	}
};

struct TraversalGraph
{
	enum Type
	{
		BreadthFirst,
		DepthFirst,

		NumTypes
	};
};

template<template<class T>class Func>
void test_graphTraversal(TraversalGraph::Type type)
{
	const int count = 10;
	NodeG<int> nodes[count];

	// init as complete binary tree first
	for(int i=0; i<count; i++)
	{
		nodes[i].data = i;

		if(2*i+1<count)
			nodes[i].neighbors.push_back(&nodes[2*i+1]);

		if(2*i+2<count)
			nodes[i].neighbors.push_back(&nodes[2*i+2]);
	}

	// add some cycle edges
	nodes[1].neighbors.push_back(&nodes[2]);
	nodes[2].neighbors.push_back(&nodes[1]);

	nodes[3].neighbors.push_back(&nodes[5]);
	nodes[5].neighbors.push_back(&nodes[3]);

	nodes[4].neighbors.push_back(&nodes[6]);
	nodes[6].neighbors.push_back(&nodes[4]);

	nodes[7].neighbors.push_back(&nodes[9]);
	nodes[9].neighbors.push_back(&nodes[7]);


	int order[TraversalGraph::NumTypes][count] =
	{
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9}
	};

	Queue<int> out;
	Func<int>::run(&nodes[0], out);

	int index = 0;
	while(!out.empty())
	{
		int data = out.dequeue();
		assert(data == order[type][index++]);
	}
}

template<class T>
bool checkTreeBalanceHelper(NodeBT<T> *root, int &height)
{
	if(!root)  //!!!!!!!!!!!!!!!!!!!!!must check based case
	{
		height = 0;  // this give leaf node height=1
		return true;
	}

	int height_left, height_right;
	if(!checkTreeBalanceHelper(root->left, height_left) ||
	   !checkTreeBalanceHelper(root->right, height_right))
		return false;

	if(std::abs(height_left-height_right) > 1)
		return false;

	height = std::max(height_left, height_right) + 1;
	return true;
}

template<class T>
bool checkTreeBalance(NodeBT<T> *root)
{
	int height;
	return checkTreeBalanceHelper(root, height);
}

void test_checkTreeBalance()
{
	NodeBT<int> nodes[6];
	for(int i=0; i<6; i++)
	{
		nodes[i].data = 0;
		nodes[i].left = nodes[i].right = NULL;
	}

	nodes[0].left  = &nodes[1];
	nodes[0].right = &nodes[2];
	nodes[2].left  = &nodes[3];
	nodes[2].right = &nodes[4];
	nodes[3].left  = &nodes[5];

	assert(!checkTreeBalance(&nodes[0]));
	assert(checkTreeBalance(&nodes[2]));
}


template<class T>
bool findNode(NodeG<T>* root, NodeG<T>* target)
{
	if(root == target) return true; //!!!!!!!!!!!!!! base case

	Queue<NodeG<T>*> q;
	q.enqueue(root);

	unordered_set<NodeG<T>*> visited;  // !!! use set
	visited.insert(root);

	while(!q.empty())
	{
		List<NodeG<T>*>::Iterator it(q.dequeue()->neighbors);
		for(; it.good(); it.advance())
		{
			NodeG<T>* node = it.value();
			if(visited.count(node) > 0)
				continue;

			if(node == target) // visit
				return true;   // !!!!!!!!!!!! success case

			visited.insert(node);
			q.enqueue(node);
		}
	}

	return false; //!!!!!!!!!!!!!!!!!! failure case
}

// Given directed graph 
template<class T>
bool checkPathExist(NodeG<T>* start, NodeG<T>* end)
{
	if(findNode(start, end) || findNode(end, start))
		return true;

	return false;
}

void test_checkPathExist()
{
	const int count = 10;
	NodeG<int> nodes[count];

	// init as complete binary tree first
	for(int i=0; i<count; i++)
	{
		nodes[i].data = i;

		if(2*i+1<count)
			nodes[i].neighbors.push_back(&nodes[2*i+1]);

		if(2*i+2<count)
			nodes[i].neighbors.push_back(&nodes[2*i+2]);
	}

	assert(checkPathExist(&nodes[0], &nodes[9]));

	nodes[4].neighbors.clear();
	assert(!checkPathExist(&nodes[0], &nodes[9]));
}

NodeBT<int>* buildMinimalBST(int a[], int start, int end) // no template!!!
{
	if(start > end) return NULL; // base case !!!!!!!!

	int middle = (start + end) / 2;

	NodeBT<int>* root = new NodeBT<int>;
	root->data = a[middle];

	root->left = buildMinimalBST(a, start, middle-1);
	root->right = buildMinimalBST(a, middle+1, end);

	return root;
}

NodeBT<int>* buildMinimalBST(int a[], int size)
{
	return buildMinimalBST(a, 0, size-1);
}

void test_buildMinimalBST()
{
	int a[10];
	for(int i=0; i<10; i++)
		a[i] = i;

	NodeBT<int>* root = buildMinimalBST(a, 10);

	Queue<int> out;
	BinaryTreeInOrderRecursion<int>::run(root, out);

	for(int i=0; i<10; i++)
		assert(a[i] == out.dequeue());

	int height;
	assert(checkTreeBalanceHelper(root, height));
	assert(height == floor(log(10.)/log(2.))+1);
}

void test_treesAndGraphs()
{
	test_binaryTreeTraversal<BinaryTreeInOrderRecursion>(TraversalBT::InOrder);
	test_binaryTreeTraversal<BinaryTreeInOrderIterative>(TraversalBT::InOrder);

	test_binaryTreeTraversal<BinaryTreePreOrderRecursion>(TraversalBT::PreOrder);
	test_binaryTreeTraversal<BinaryTreePreOrderIterative>(TraversalBT::PreOrder);

	test_binaryTreeTraversal<BinaryTreePostOrderRecursion>(TraversalBT::PostOrder);
	//test_binaryTreeTraversal<BinaryTreePostOrderIterative>(TraversalBT::PostOrder); // implement this
	
	test_binaryTreeTraversal<BinaryTreeLevelOrder>(TraversalBT::LevelOrder);

	test_graphTraversal<GraphBreadthFirst>(TraversalGraph::BreadthFirst);
	//test_graphTraversal<GraphDepthFirstRecursion>(TraversalGraph::DepthFirst); // implement this
	//test_graphTraversal<GraphDepthFirstIterative>(TraversalGraph::DepthFirst); // implement this

	test_checkTreeBalance();
	test_checkPathExist();
	test_buildMinimalBST();
}