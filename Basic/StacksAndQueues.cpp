#include "Common.h"
#include <functional> // std::less
#include <assert.h>

template<class T>
struct ThreeStacksFixed
{
private:
	int* buffer;  // !!!!!!!! can't define as "int buffer[]"
	int stack_size;
	int stack_pointers[3];

public:
	ThreeStacksFixed(int stack_size_=100)
	{
		stack_size = stack_size_;
		buffer = new int[stack_size * 3];
		for(int i=0; i<3; i++)
			stack_pointers[i] = -1;
	}

	~ThreeStacksFixed()
	{
		delete buffer;
	}

	int getBufferPointer(int stack_id, int stack_pointer)
	{
		return stack_id * stack_size + stack_pointer;
	}

	void push(int stack_id, T data)
	{
		int sp = stack_pointers[stack_id];
		if(sp + 1 > stack_size)
			std::abort();

		sp += 1;
		int bp = getBufferPointer(stack_id, sp);
		buffer[bp] = data;
		stack_pointers[stack_id] = sp;
	}

	T pop(int stack_id)
	{
		int sp = stack_pointers[stack_id];
		if(sp < 0)
			std::abort();

		int bp = getBufferPointer(stack_id, sp);
		T data = buffer[bp];
		stack_pointers[stack_id] -= 1;
		return data;
	}

	bool empty(int stack_id)
	{
		return stack_pointer[stack_id] == -1;
	}
};

template<template<class T>class S>
void test_ThreeStacks()
{
	S<int> stack;

	// Init
	int a[20], b[20], c[20];
	for(int i=0; i<20; i++)
	{
		a[i] = i * 5;
		b[i] = i * 10;
		c[i] = i * 15;
	}

	// Push
	for(int i=0; i<20; i++)
	{
		stack.push(0, a[i]);
		stack.push(1, b[i]);
		stack.push(2, c[i]);
	}

	// Pop and check
	for(int i=0; i<20; i++)
	{
		int data = stack.pop(0);
		assert(data == a[19-i]);
	}

	for(int i=0; i<20; i++)
	{
		int data = stack.pop(1);
		assert(data == b[19-i]);
	}

	for(int i=0; i<20; i++)
	{
		int data = stack.pop(2);
		assert(data == c[19-i]);
	}
}

template<class T, typename TLess>
struct MinStack
{
private:
	NodeS<T> *head;
	NodeS<T> *min_head;

public:  //!!!!!!!!!!!!!!!!!!!!
	MinStack() : head(NULL), min_head(NULL) {}

	void push(T data)
	{
		NodeS<T> *node = new NodeS<T>;  //!!!!!!!!!!!!!!!!!
		node->data = data;
		node->next = head;

		NodeS<T> *min_node = new NodeS<T>;
		min_node->next = min_head;
		if(!min_head || !TLess()(min_head->data, data))  //!!!!!!! create comparison object every time
			min_node->data = data;
		else
			min_node->data = min_head->data;  // min_head->data could just call min()

		head = node;
		min_head = min_node;
	}

	T pop()
	{
		if(!head)
			std::abort();

		T data = head->data;
		NodeS<T>* node = head->next;
		delete head;
		head = node;

		NodeS<T>* min_node = min_head->next;
		delete min_head;
		min_head = min_node;

		return data;
	}

	T min()
	{
		if(!min_head)
			std::abort();

		return min_head->data;
	}

	bool empty()
	{
		return !head;
	}
};

template<template<class T, typename TLess>class S>
void test_minStack()
{
	S<int, IntLess> stack;

	stack.push(5);
	stack.push(7);
	stack.push(6);
	stack.push(3);
	assert(3 == stack.min());

	stack.push(2);
	stack.push(4);
	assert(2 == stack.min());

	stack.pop();
	stack.pop();
	stack.pop();
	assert(5 == stack.min());
}

void test_stacksAndQueues()
{
	test_ThreeStacks<ThreeStacksFixed>();
	//test_ThreeStacks<ThreeStacksFlexible>(); // implement with flexible division of buffer

	test_minStack<MinStack>();
	//test_minStack<MinStackWithSecondStack>() // implement with second stack to track min value
}