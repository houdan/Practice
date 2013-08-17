#include <math.h>
#include <assert.h>

int divide2(int a, int b)
{
	assert(b != 0);

	if(a == 0 || b == 1) return a; // base case
	if(b == -1) return -a; // base case

	long long abs_a = abs((double)a);  // !!!!!!!!! if b == INT_MIN (abs overflows) or INT_MAX (d << 1 overflows)
	long long abs_b = abs((double)b);  // !!!!!!!!! abs() doesn't have overload for long long

	int c = 0; // quotient  // !!!!!! if b = -1 and a = INT_MIN, overflows
	while(abs_a >= abs_b) // final abs_a is remainder 
	{
		long long d = abs_b; // start with smallest divisor
		for(int i=0; abs_a >= d; i++)
		{
			abs_a -= d;
			c += 1 << i;
			d = d << 1; // double divisor
		}
	}

	return ((a^b) >> 31) ? -c : c;  // !!! check the sign with ^, note >> take precedence over ^
}

void test_divide2()
{
	assert(divide2(0, 5) == 0);
	assert(divide2(0, -5) == 0);
	assert(divide2(5, 5) == 1);
	assert(divide2(-5, -5) == 1);
	assert(divide2(5, -5) == -1);
	assert(divide2(-5, 5) == -1);
	assert(divide2(9, 5) == 1);
	assert(divide2(9, -5) == -1); 
}

void test_bits()
{
	// Implement div without mul and mod
	test_divide2();
}