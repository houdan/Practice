#include "List.h"
#include "Common.h"
#include <assert.h>

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