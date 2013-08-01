#include "Common.h"
#include <unordered_set>
#include <assert.h>

using namespace std;

// Implementation allows buffer
template <class T>
struct RemoveDuplicate1
{
	static void Run(NodeS<T> *head)
	{
		if(!head)
			return;

		unordered_set<T> vals; // !!! no need for map
		NodeS<T>* curr = head;   // !!! don't miss <T>

		vals.insert(curr->data);
		while(curr->next)
		{
			NodeS<T>* next = curr->next;
			if(vals.count(next->data)) // !!! use count
			{
				curr->next = next->next;
				// maybe free up memory
			}
			else
			{
				vals.insert(next->data);
				curr = curr->next; // !!!!!!!!! don't move when next is removed
			}
		}
	}
};

// Implementation using two pointers
template <class T>
struct RemoveDuplicate2
{
	static void Run(NodeS<T> *head)
	{
		NodeS<T> *runner, *curr = head;
		while(curr) // also eliminate head==NULL case
		{
			runner = curr;
			while(runner->next)
			{
				if(runner->next->data == curr->data)
				{
					runner->next = runner->next->next;
					// maybe free up memory
				}
				else
					runner = runner->next; // !!! don't move runner when next is removed
			}
			curr = curr->next;
		}
	}
};

// !!! can't pass templated functoin as a templated type, since it 
// must be instantiated first, so wrap it up with a struct
template<template <class T> class Func>   
void test_removeDuplicate() 
{
	// Test NULL
	NodeS<int> *head = NULL;
	Func<int>::Run(head);

	// Test single item
	NodeS<int> node = {1, NULL};
	Func<int>::Run(&node);
	assert(node.data == 1);

	// Test multiple items
	int vals_a[8] = {0, 0, 1, 2, 3, 3, 3, 4};
	int vals_b[5] = {0, 1, 2, 3, 4};

	NodeS<int> nodes[8];
	for(int i=0; i<8; i++)
	{
		nodes[i].data = vals_a[i];
		if(i != 7)
			nodes[i].next = &nodes[i+1];
		else
			nodes[i].next = NULL;
	}

	Func<int>::Run(nodes);

	int index = 0;
    NodeS<int> *curr = nodes;
	while(curr)
	{
		assert(curr->data == vals_b[index++]);
		curr = curr->next;
	}
	assert(index == 5); //!!!!!!!!
}

// Implement with two pointers
template<class T>
struct FindKthToLast1
{
	static NodeS<T>* Run(NodeS<T>* head, int k)
	{
		NodeS<T> *curr, *runner; //!!!!!!!!!!
		curr = runner = head;

		int steps = 0;
		while(runner)
		{
			// !!!!!!!!!!!! move k steps, since  
			// runner will move pass the last item
			if(steps < k)
			{	
				runner = runner->next;
				steps++;
			}
			else
			{
				runner = runner->next;
				curr = curr->next;
			}
		}

		if(steps < k)
			return NULL;
		else
			return curr;
	}
};

// Implementation using recursion
template<class T>
struct FindKthToLast2
{
	static NodeS<T>* Find(NodeS<T>* head, int k, int &n)
	{
		if(head == NULL)
			return NULL;

		NodeS<T>* node = Find(head->next, k, n); //!!!!!!!!!!!!! head->next
		
		n += 1;
		if(n == k)
			return head;
		else
			return node;
	}

	static NodeS<T>* Run(NodeS<T>* head, int k)
	{
		int n = 0;
		return Find(head, k, n);
	}
};

template<template<class T> class Func>
void test_findKthToLast()
{
	NodeS<int> nodes[10];
	for(int i=0; i<10; i++)
	{
		nodes[i].data = i;
		nodes[i].next = (i<9)? &nodes[i+1] : NULL; //!!!!!!!
	}

    // success case
	NodeS<int>* kth = Func<int>::Run(nodes, 7);
	assert(kth && kth->data == 3);

	// failure case
	kth = Func<int>::Run(nodes, 11);
	assert(!kth);
}

template<class T>
bool removeNodeWithoutHead(NodeS<T> *node)
{
	if(!node || !(node->next))
		return false;  // !!!!!!!!! tail node can't be remove, since prev node must be modified 

	*node = *(node->next);
	// maybe free up memory

	return true;
}

void test_removeNodeWithoutHead()
{
	int vals_a[5] = {0, 1, 2, 3, 4};
	int vals_b[4] = {0, 1, 2, 4};

	NodeS<int> nodes[5];
	for(int i=0; i<5; i++)
	{
		nodes[i].data = vals_a[i];
		nodes[i].next = (i<4)? &nodes[i+1] : NULL;
	}

	bool sucess = removeNodeWithoutHead(&nodes[4]);
	assert(!sucess);

	sucess = removeNodeWithoutHead(&nodes[3]);
	assert(sucess);

	int index = 0;
	NodeS<int>* curr = nodes;
	while(curr)
	{
		assert(curr->data == vals_b[index++]);
		curr = curr->next;
	}
	assert(index == 4);
}

template<class T>
NodeS<T>* partitionOnX(NodeS<T>* head, T x)
{
	NodeS<T> *small_head, * large_head;
	small_head = large_head = NULL;

	NodeS<T> *curr = head;
	while(curr)
	{
		NodeS<T>* next = curr->next;

		// Insert from the head
		if(curr->data < x)
		{
			curr->next = small_head;
			small_head = curr;
		}
		else
		{
			curr->next = large_head;
			large_head = curr;
		}

		curr = next; // !!!!!!!!!!!!!!!!!!!!!! don't forget
	}

	if(!small_head)
		return large_head;

	curr = small_head;
	while(curr->next)     // !!!!!! could record the small_tail to avoid this loop
		curr = curr->next;
	curr->next = large_head;

	return small_head;
}

void test_partitionOnX()
{
    int vals_a[8] = {0, 3, 7, 6, 4, 1, 5, 2};

	NodeS<int> nodes[8];
	for(int i=0; i<8; i++)
	{
		nodes[i].data = vals_a[i];
		nodes[i].next = (i<7)? &nodes[i+1] : NULL;
	}

	int x = 4;
	NodeS<int>* head = partitionOnX(nodes, x);

	bool larger = false;
	while(head)
	{
		if(!larger && head->data >= x)
			larger = true;

		assert((head->data >= x) == larger);
		head = head->next;
	}
}

// Caller is responsible for freeing up the memory
// Use recursion will be simpler
template<class T>
NodeS<T>* addTwoReverse(NodeS<T>* n1, NodeS<T>* n2)
{
	NodeS<T> *head, *tail;  // !!!!! * before each pointer
	head = tail = NULL; // !!!!!!!! can't initialize with a node

	bool addone = false;
	while(n1 || n2 || addone) //!!!!!!!!!!!!!!! addone needs to be checked too
	{
		int c = 0;
		if(n1)
		{
			c += n1->data;
			n1 = n1->next;
		}
		if(n2)
		{
			c += n2->data;
			n2 = n2->next;
		}
		if(addone)
			c += 1;
		
		if(c >= 10)
		{
			c = c % 10;
			addone = true;
		}
		else
			addone = false;

		NodeS<T>* node = new NodeS<T>;
		node->next = NULL;
		node->data = c;

		if(!head)  // insert at end, must check NULL
		{
			head = node;
			tail = head;
		}
		else
		{
			tail->next = node;
			tail = node;
		}
	}

	return head;
}

void test_addTwoReverse()
{
	int vals_a[8] = {6, 3, 7, 6, 4, 1, 5, 2};
	int vals_b[5] = {2, 9, 5, 1, 8};
	int vals_c[8] = {8, 2, 3, 8, 2, 2, 5, 2};

	NodeS<int> nodes_a[8];
	for(int i=0; i<8; i++)
	{
		nodes_a[i].data = vals_a[i];
		nodes_a[i].next = (i<7)? &nodes_a[i+1] : NULL;
	}

	NodeS<int> nodes_b[5];
	for(int i=0; i<5; i++)
	{
		nodes_b[i].data = vals_b[i];
		nodes_b[i].next = (i<4)? &nodes_b[i+1] : NULL;
	}

	NodeS<int>* sum = addTwoReverse(nodes_a, nodes_b);
	
	int index = 0;
	while(sum)
	{
		assert(sum->data == vals_c[index++]);

		NodeS<int>* curr = sum;
		sum = sum->next;
		delete curr;
	}
	assert(index == 8);
}

// This implements with hash table of pointers
template<class T>
struct FindLoopStart1
{
	static NodeS<T>* Run(NodeS<T>* n)
	{
		unordered_set<NodeS<T>*> seen;
		while(n)
		{
			if(seen.count(n))
				return n;
			else
				seen.insert(n); //!!!!!!!!!!!!!!!!!!!!
			n = n->next;
		}
		return NULL;
	}
};

template<template<class T>class Func>
void test_findLoopStart()
{
	int vals_a[8] = {6, 3, 7, 6, 4, 1, 5, 2};

	NodeS<int> nodes_a[8];
	for(int i=0; i<8; i++)
	{
		nodes_a[i].data = vals_a[i];
		nodes_a[i].next = (i<7)? &nodes_a[i+1] : NULL;
	}

	int loop_start_index = 5;
	nodes_a[7].next = &nodes_a[loop_start_index];

	NodeS<int>* start = Func<int>::Run(nodes_a);

	assert(start == &nodes_a[loop_start_index]);
}

void test_linkedLists()
{
	test_removeDuplicate<RemoveDuplicate1>();
	test_removeDuplicate<RemoveDuplicate2>();

	test_findKthToLast<FindKthToLast1>();
	test_findKthToLast<FindKthToLast2>();

	test_removeNodeWithoutHead();
	test_partitionOnX();

	test_addTwoReverse();
	//test_addTwoForward(); // TODO: implement it, need padding, recursion with carry-on value passed back via reference

	test_findLoopStart<FindLoopStart1>();
	//test_findLoopStart<FindLoopStart1>(); // TODO: implement two pointers version
}