#include "Common.h"
#include <vector>
#include <assert.h>

using namespace std;

vector<int> intersectTwoSortedArray(const vector<int> &a, const vector<int> &b)
{
	vector<int> intersection;
	int index_a = 0, index_b = 0;

	while(index_a < a.size() && index_b < b.size())
	{
		int val_a = a[index_a];
		int val_b = b[index_b];

		if(val_a == val_b)
		{
			intersection.push_back(val_a);
			index_a++;
			index_b++;
		}
		else if (val_a < val_b)
			index_a++;
		else
			index_b++;
	}

	return intersection;
}

void test_intersectTwoSortedArray()
{
	vector<int> a, b;
	for(int i = 0; i<10; i++)
	{
		a.push_back(i);
		b.push_back(i*2);
	}

	vector<int> ab = intersectTwoSortedArray(a, b);
	assert(ab.size() == 5);
	for(int i=0; i<5; i++)
		assert(ab[i] == i*2);
}


void test_array()
{
	// Rotate Array
	// note: three reverse

	// Merge sorted array - done

	// Find intersection of two sorted arrays
	// note: two pointers (or hashtable, or binary search)
	test_intersectTwoSortedArray();

	// Find K-th Smallest Element of Two Sorted Arrays (with no duplicates)
	// note: binary search runtime O(logm+logn) (or merge, or two pointers)

	// Find Median of Two Sorted Arrays
	// note: runtime O(log(m+n))

	// Two-Sum

	// Three-Sum

	// Four-Sum
}