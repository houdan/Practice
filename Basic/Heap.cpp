#include "Heap.h"
#include <assert.h>

void test_heap()
{
	// test min heap
	{
		Heap<int, IntLessOrEqual> minHeap;

		const int count = 100;
		for(int i=0; i<count; i++)
		{
			minHeap.insert(rand()%count);
		}

		int n = 0;
		int last_min = INT_MIN;
		while(!minHeap.empty())
		{
			assert(last_min <= minHeap.peek());
			last_min = minHeap.pop();
			n += 1;
		}
		assert(n == count);
	}

	// test max heap
	{
		Heap<int, IntGreaterOrEqual> maxHeap;

		const int count = 100;
		for(int i=0; i<count; i++)
		{
			maxHeap.insert(rand()%count);
		}

		int n = 0;
		int last_max = INT_MAX;
		while(!maxHeap.empty())
		{
			assert(last_max >= maxHeap.peek());
			last_max = maxHeap.pop();
			n += 1;
		}
		assert(n == count);
	}
}

