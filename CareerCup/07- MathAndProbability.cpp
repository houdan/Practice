#include <assert.h>
#include <cmath>

struct IsPrimeSimple
{
	static bool run(int n)
	{
		if(n<2) return false;

		int m = 2;
		while(m < n)
		{
			if(n % m == 0)
				return false;
			m++;
		}
		return true;
	}
};

struct IsPrimeWithSqrt
{
	static bool run(int n)
	{
		if(n<2) return false;

		int m = 2;
		int sqt = (int)std::sqrt((double)n);
		while(m <= sqt)
		{
			if(n % m == 0)
				return false;
			m++;
		}
		return true;
	}
};

struct IsPrimeWithEnumeration
{
	static void setNonPrimes(bool flags[], int size, int p)
	{
		for(int i=p*p; i<size; i+=p) // !!!!!! p*k for k<p has been set previously
			flags[i] = false;
	}

	static int getNextPrime(bool flags[], int size, int p)
	{
		int next = p + 1;
		while(next < size && !flags[next])  //!!!!!!!!! could only use odd numbers
		{
			next++; 
		}

		return next; // !!!!!! note this may return "size+1", but doesn't affect result
	}

	static bool run(int n)
	{
		bool *flags = new bool[n+1];   

		flags[0] = flags[1] = false;
		for(int i=2; i<n+1; i++)
			flags[i] = true;

		int m = 2;
		int sqt = (int)std::sqrt((double)n);
		while(m <= sqt)
		{
			setNonPrimes(flags, n+1, m);
			m = getNextPrime(flags, n+1, m);  // !!!!!!! update m;
		}

		bool isPrime = flags[n]; // last element
		delete flags;

		return isPrime;
	}

};

template<class Func>
void test_isPrime()
{
	assert(!Func::run(1));
	assert(Func::run(2));
	assert(Func::run(29));
	assert(Func::run(113));
	assert(!Func::run(125));
}

void test_mathAndProbability()
{
	test_isPrime<IsPrimeSimple>();
	test_isPrime<IsPrimeWithSqrt>();
	test_isPrime<IsPrimeWithEnumeration>();
}