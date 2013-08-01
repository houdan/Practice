#include "Queue.h"
#include "Common.h"
#include <assert.h>

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