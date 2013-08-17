#include "Sorting.h"
#include <algorithm>
#include <assert.h>

template<class T>
void test_comparisonSorting(T &t)
{
	const int n = 1000;

	int a[n];
	for(int i=0; i<n; i++)
		a[i] = i;

	std::random_shuffle(&a[0], &a[n-1]);
	t.sort(a, n, IntLessOrEqual());

	for(int i=0; i<n; i++)
		assert(a[i] == i);
}


void test_sorting()
{
	// O(n^2) 
	test_comparisonSorting(BubbleSort());
	test_comparisonSorting(SelectionSort());
	test_comparisonSorting(InsertionSort());

	// O(nlogn)
	test_comparisonSorting(MergeSort());
	test_comparisonSorting(QuickSort());
	test_comparisonSorting(HeapSort());
}