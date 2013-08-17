#include <assert.h>

// not handling overflow
int neg(int a)
{
	int n = 0;
	int d = a > 0 ? -1 : 1;
	while(a != 0)
	{
		n += d;
		a += d;
	}
	return n;
}

// not handling overflow
int subtract(int a, int b)
{
	return a + neg(b);	
}

void test_subtract()
{
	assert(subtract(0, 0) == 0);
	assert(subtract(5, 5) == 0);
	assert(subtract(-5, -5) == 0);
	assert(subtract(5, -5) == 10);
	assert(subtract(-5, 5) == -10);
}

// not handling overflow
int abs(int a)
{
	return (a < 0) ? neg(a) : a; 
}

// not handling overflow
int multiply(int a, int b)
{
	int abs_a = abs(a);
	int abs_b = abs(b);

	if(abs_a < abs_b)
		multiply(b, a); // more efficient

	int sum = 0;
	for(int i=0; i<abs_b; i++)
		sum += a;

	return b < 0 ? neg(sum) : sum;
}

void test_multiply()
{
	assert(multiply(0, 0) == 0);
	assert(multiply(5, 0) == 0);
	assert(multiply(0, -5) == 0);
	assert(multiply(5, 5) == 25);
	assert(multiply(-5, -5) == 25);
	assert(multiply(5, -5) == -25);
	assert(multiply(-5, 5) == -25);
}

// not handling overflow
int divide(int a, int b)
{
	assert(b != 0);

	if(a == 0 || b == 1) return a; // base case
	if(b == -1) return neg(a); // base case

	int abs_a = abs(a);
	int abs_b = abs(b);

	int x = 0;
	int sum = abs_b; 
	while(sum <= abs_a) // round towards 0
	{
		x += 1;  // !!!! update x to match current sum
		sum += abs_b;
	}

	bool sign_a = a > 0;
	bool sign_b = b > 0;

	return (sign_a == sign_b) ? x : neg(x);
}

void test_divide()
{
	assert(divide(0, 5) == 0);
	assert(divide(0, -5) == 0);
	assert(divide(5, 5) == 1);
	assert(divide(-5, -5) == 1);
	assert(divide(5, -5) == -1);
	assert(divide(-5, 5) == -1);
	assert(divide(9, 5) == 1);
	assert(divide(9, -5) == -1); 
}

int reverseInteger(int x)
{
	bool isNeg = x < 0;
	x = abs(x);

	int result = 0;
	while(x > 0)
	{
		result = result*10 + x%10;
		x = x/10;
	}

	// check overflow 
	if(result < 0) return 10; // !!! 10,100,...reverse to 1, but nothing reverse to 10, 100,...

	return isNeg ? -1*result : result; // !!! result, not x
}

void test_reverseInteger()
{
	assert(reverseInteger(0) == 0);
	assert(reverseInteger(1) == 1);
	assert(reverseInteger(10) == 1);
	assert(reverseInteger(123) == 321);
	assert(reverseInteger(-1) == -1);
	assert(reverseInteger(-10) == -1);
	assert(reverseInteger(1000000003) == 10);
}

struct IsPalindromeIterative
{
	static bool run(int x)
	{
		if(x < 0) return false;
		if(x == 0) return true;

		// find divisor to get first digit
		int div = 1;
		while(x / div >= 10) // !!!!!!!! use >=
			div *= 10;

		while(x > 0)
		{
			// compare fist and last digit
			if(x / div != x % 10)
				return false;

			// chop off first and last digit 
			x = (x % div) / 10;
			div /= 100; // !!!!!!!! update
		}
		return true;
	}
};

struct IsPalindromeRecursion
{
	static bool isPalindrome(int x, int &y)
	{
		if(x < 0) return false;
		if(x == 0) return true;
		
		// use recursion stack to chop off first digit
		if(isPalindrome(x/10, y) && (x%10 == y%10)) // use x%10 to get first digit, y%10 to get last digit
		{
			y /= 10; // use this to chop off last digit
			return true;
		}
		else
			return false;
	}

	static bool run(int x)
	{
		return isPalindrome(x, x);
	}
};

template<class Func>
void test_isPalindrome()
{
	assert(!Func::run(-1));
	assert(Func::run(0));
	assert(Func::run(1));
	assert(!Func::run(10));
	assert(Func::run(11));
    assert(Func::run(121));
	assert(Func::run(1221));
}


void test_integer()
{
	// Implement sub with only add
	test_subtract();

	// Implement mul with only add
	test_multiply();

	// Implement div with only add
	test_divide();

	// Reverse digits of an integer
	test_reverseInteger();

	// Determine whether an integer is a palindrome. Do this without extra space
	test_isPalindrome<IsPalindromeIterative>();
	test_isPalindrome<IsPalindromeRecursion>();
}