#include "Common.h"
#include <algorithm>
#include <assert.h>

using namespace std;

struct BubbleSort
{
	// TComparer returns true is first argument should go before second argument
	template<class T, class TComparer> 
	static void run(T a[], int n, TComparer &comp)	
	{
		for(int i=0; i<n; i++)
			for(int j=0; j<n-i-1; j++)
			{
				if(!comp(a[j], a[j+1]))
				{
					swap(a[j], a[j+1]);
				}
			}	
	}
};

struct SelectionSort
{
	template<class T, class TComparer>
	static void run(T a[], int n, TComparer &comp)
	{
		for(int i=0; i<n; i++)
		{
			int j_min = i;  // !!!!!!!!!!!! avoid checking initial condition inside for loop
			for(int j=i+1; j<n; j++)
			{
				if(!comp(a[j_min], a[j]))
					j_min = j;
			}

			if(j_min != i)
				swap(a[j_min], a[i]);  // select correct element
		}
	}
};

struct InsertionSort
{
	template<class T, class TComparer>
	static void run(T a[], int n, TComparer &comp)
	{
		for(int i=1; i<n; i++)
		{
			T val = a[i];
			int index = i-1;
			while(index >= 0 && comp(val, a[index]))
			{
				a[index+1] = a[index];  // shift it to right 
				index -= 1;
			}
			a[index+1] = val; // insert
		}
	}
};

// Implement top-down version with recursion
struct MergeSort
{
	template<class T, class TComparer>
	static void merge(T a[], T buffer[], int start, int middle, int end, TComparer &comp) // !!! left half a[start]-a[middle] and right half a[middle+1]-a[end]
	{
		// copy array into buffer
		for(int i=start; i<=end; i++)  //!!!!!!!! <=end
			buffer[i] = a[i];

		int index = start;
		int index_left = start;
		int index_right = middle+1;

		// repeat until either half runs out
		while(index_left <= middle && index_right <= end) 
		{
			if(comp(buffer[index_left], buffer[index_right]))
			{
				a[index++] = buffer[index_left++];
			}
			else
			{
				a[index++] = buffer[index_right++];
			}
		}

		// only need to copy left half back !!!!!!!!!!
		while(index_left <= middle)
		{
			a[index++] = buffer[index_left++];
		}
	}

	template<class T, class TComparer>
	static void sort(T a[], T buffer[], int start, int end, TComparer &comp) // !!! create buffer once and pass in
	{
		if(start >= end)   // !!!!!!!! base case
			return;   

		int middle = (start + end) / 2; 

		sort(a, buffer, start, middle, comp);
		sort(a, buffer, middle+1, end, comp);
		merge(a, buffer, start, middle, end, comp);
	}

	template<class T, class TComparer>
	static void run(T a[], int n, TComparer &comp)
	{
		T* buffer = new T[n];
		sort(a, buffer, 0, n-1, comp);
		delete buffer;
	}
};

// Implement in-place version
struct QuickSort
{
	template<class T, class TComparer>
	static int partition(T a[], int start, int end, int pivot, TComparer &comp)
	{
		const T p_val = a[pivot];

		// Move pivot to the end
		swap(a[pivot], a[end]);

		// Move two pointer from two ends to middle
		int l = start;   // left of l must come before pivot
		int r = end-1;   // right of r must come after pivot

		while(l <= r) // !!!!!! left must pass right
		{
			while(l <= r && comp(a[l], p_val)) l++;

			while(l <= r && comp(p_val, a[r])) r--;

			if(l < r)
			{
				swap(a[l++], a[r--]);
			}
		}

		// Move pivot back 
		swap(a[end], a[l]); // l now pointing to first value that come after pivot

		return l;
	}

	template<class T, class TComparer>
	static void sort(T a[], int start, int end, TComparer &comp)
	{
		if(start >= end) // !!!!!!!!! base case
			return;

		int pivot = start + std::rand() % (end - start + 1);  // !!!!!!!! maybe other choices
		int index = partition(a, start, end, pivot, comp);

		sort(a, start, index-1, comp);
		sort(a, index+1, end, comp);
	}

	template<class T, class TComparer>
	static void run(T a[], int n, TComparer &comp)
	{
		sort(a, 0, n-1, comp);	
	}
};

template<class Func>
void test_comparisonSorting()
{
	const int count = 1000;

	int a[count];
	for(int i=0; i<count; i++)
		a[i] = i;

	std::random_shuffle(&a[0], &a[count-1]);
	Func::run(a, count, IntLessOrEqual());

	for(int i=0; i<count; i++)
		assert(a[i] == i);
}

//void test_integerSorting()
//{
//	
//}

void test_sortingAndSearching()
{
	test_comparisonSorting<BubbleSort>();
	test_comparisonSorting<SelectionSort>();
	test_comparisonSorting<InsertionSort>();

	test_comparisonSorting<MergeSort>();
	test_comparisonSorting<QuickSort>();
	
}