#include <assert.h>

int getBit(int n, int i)
{
	return (n & (1<<i)) != 0;	
}

// set to 1
int setBit(int n, int i)
{
	return n | (1<<i);
}

int clearBit(int n, int i)
{
	return n & ~(1<<i);
}

int updateBit(int n, int i, int v)
{
	return n & ~(1<<i) | (v<<i);
}

void test_bitTasks()
{
	assert(getBit(4,1) == 0);
	assert(getBit(4,2) == 1);

	assert(setBit(4,1) == 6);
	assert(setBit(4,2) == 4);

	assert(clearBit(4,1) == 4);
	assert(clearBit(4,2) == 0);

	assert(updateBit(4,1,0) == 4);
	assert(updateBit(4,2,1) == 4);
}

void test_bitManipulation()
{
	test_bitTasks();
}