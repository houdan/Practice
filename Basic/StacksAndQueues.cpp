#include "Common.h"
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

void test_stacksAndQueues()
{
	test_ThreeStacks<ThreeStacksFixed>();
	//test_ThreeStacks<ThreeStacksFlexible>(); // implement with flexible division of buffer
}