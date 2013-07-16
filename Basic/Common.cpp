#include "Common.h"
#include <assert.h>

void test_stack()
{
	Stack<int> stack;

	for(int i=0; i<10; i++)
		stack.push(i);

	assert(!stack.empty());

	for(int i=0; i<10; i++)
	{
		int data = stack.pop();
		assert(data == 9-i);
	}

	assert(stack.empty());
}

void test_queue()
{
    Queue<int> queue;

	for(int i=0; i<10; i++)
		queue.enqueue(i);

	assert(!queue.empty());

	for(int i=0; i<10; i++)
	{
		int data = queue.dequeue();
		assert(data == i);
	}

	assert(queue.empty());
}

void test_list()
{
	List<int> a, b;
	for(int i=0; i<10; i++)
	{
		a.push_back(i);
		b.push_front(i);
	}

	{
		int counter = 0;
		List<int>::Iterator it(a);
		for(; it.good(); it.advance())
		{
			assert(it.value() == counter++);
		}
	}

	{
		int counter = 9;
		List<int>::Iterator it(b);
		for(; it.good(); it.advance())
		{
			assert(it.value() == counter--);
		}
	}


	{
		a.clear();

		int count = 0;
		List<int>::Iterator it(a);
		for(; it.good(); it.advance())
			count++;

		assert(count == 0);
	}
}

void test_common()
{
	test_stack();
	test_queue();
	test_list();
}