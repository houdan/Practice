#include "BitSet.h"
#include <algorithm>
#include <assert.h>

using namespace std;

void test_bitset()
{
	const int count = 1000;
	unsigned short a[count];
	for(int i=0; i<count; i++)
		a[i] = i;

	random_shuffle(a, a+count);

	BitSet bs(2 << 16);
	for(int i=0; i<count; i++)
		bs.Set(a[i]);

	int index = 0;
	for(int i=0; i<bs.Size(); i++)
	{
		if(bs.Get(i))
			a[index++] = i;
	}

	assert(index == count);
	for(int i=0; i<count; i++)
		assert(a[i] == i);
}