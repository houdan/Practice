#include <vector>
#include <assert.h>

using namespace std;

int computeFibonacci(int n, vector<int> &cache)
{
	if(n==0) return 0;
	if(n==1) return 1;
	if(cache[n] > 0) return cache[n];
	cache[n] = computeFibonacci(n-1, cache) + computeFibonacci(n-2, cache);
	return cache[n];  //!!!!!!!!!!!!!
}

// note this method will overflow for large n
int fibonacci(int n)
{
	vector<int> cache(n+1, 0);
	return computeFibonacci(n, cache);
}

void test_fibonacci()
{
	assert(0 == fibonacci(0));
	assert(1 == fibonacci(1));
	assert(55 == fibonacci(10));
	assert(6765 == fibonacci(20));
}

void test_recursionAndDP()
{
	test_fibonacci();
}