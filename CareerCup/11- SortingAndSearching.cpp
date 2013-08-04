#include "Common.h"
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

using namespace std;

struct BubbleSort
{
	// Tcomparer returns true is first argument should go before second argument
	template<class T, class Tcomparer> 
	static void run(T a[], int n, Tcomparer &cmp)	
	{
		for(int i=0; i<n; i++)
			for(int j=0; j<n-i-1; j++)
			{
				if(!cmp(a[j], a[j+1]))
				{
					swap(a[j], a[j+1]);
				}
			}	
	}
};

struct SelectionSort
{
	template<class T, class Tcomparer>
	static void run(T a[], int n, Tcomparer &cmp)
	{
		for(int i=0; i<n; i++)
		{
			int j_min = i;  // !!!!!!!!!!!! avoid checking initial condition inside for loop
			for(int j=i+1; j<n; j++)
			{
				if(!cmp(a[j_min], a[j]))
					j_min = j;
			}

			if(j_min != i)
				swap(a[j_min], a[i]);  // select correct element
		}
	}
};

struct InsertionSort
{
	template<class T, class Tcomparer>
	static void run(T a[], int n, Tcomparer &cmp)
	{
		for(int i=1; i<n; i++)
		{
			T val = a[i];
			int index = i-1;
			while(index >= 0 && cmp(val, a[index]))
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
	template<class T, class Tcomparer>
	static void merge(T a[], T buffer[], int start, int middle, int end, Tcomparer &cmp) // !!! left half a[start]-a[middle] and right half a[middle+1]-a[end]
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
			if(cmp(buffer[index_left], buffer[index_right]))
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

	template<class T, class Tcomparer>
	static void sort(T a[], T buffer[], int start, int end, Tcomparer &cmp) // !!! create buffer once and pass in
	{
		if(start >= end)   // !!!!!!!! base case
			return;   

		int middle = (start + end) / 2; 

		sort(a, buffer, start, middle, cmp);
		sort(a, buffer, middle+1, end, cmp);
		merge(a, buffer, start, middle, end, cmp);
	}

	template<class T, class Tcomparer>
	static void run(T a[], int n, Tcomparer &cmp)
	{
		T* buffer = new T[n];
		sort(a, buffer, 0, n-1, cmp);
		delete buffer;
	}
};

// Implement in-place version
struct QuickSort
{
	template<class T, class Tcomparer>
	static int partition(T a[], int start, int end, int pivot, Tcomparer &cmp)
	{
		const T p_val = a[pivot];

		// Move pivot to the end
		swap(a[pivot], a[end]);

		// Move two pointer from two ends to middle
		int l = start;   // left of l must come before pivot
		int r = end-1;   // right of r must come after pivot

		while(l <= r) // !!!!!! left must pass right
		{
			while(l <= r && cmp(a[l], p_val)) l++;

			while(l <= r && cmp(p_val, a[r])) r--;

			if(l < r)
			{
				swap(a[l++], a[r--]);
			}
		}

		// Move pivot back 
		swap(a[end], a[l]); // l now pointing to first value that come after pivot

		return l;
	}

	template<class T, class Tcomparer>
	static void sort(T a[], int start, int end, Tcomparer &cmp)
	{
		if(start >= end) // !!!!!!!!! base case
			return;

		int pivot = start + std::rand() % (end - start + 1);  // !!!!!!!! maybe other choices
		int index = partition(a, start, end, pivot, cmp);

		sort(a, start, index-1, cmp);
		sort(a, index+1, end, cmp);
	}

	template<class T, class Tcomparer>
	static void run(T a[], int n, Tcomparer &cmp)
	{
		sort(a, 0, n-1, cmp);	
	}
};

struct HeapSort
{
	template<class T, class Tcomparer>
	static void run(T a[], int n, Tcomparer &cmp)
	{
		// convert to a heap
		heapify(a, n, cmp);

		int end = n-1;
		while(end > 0)
		{
			// pop from heap and place it at the end (!!! reverse order) 
			swap(a[0], a[end]); 

			// put heap back in order
			end -= 1;
			siftDown(a, end+1, 0, cmp);
		}

		// convert back to correct order (!!! not needed if using opposite comparer to build the heap)  
		int l = 0, r = n-1;
		while(l < r) swap(a[l++], a[r--]);
	}
};

template<class Func>
void test_comparisonSorting()
{
	const int n = 1000;

	int a[n];
	for(int i=0; i<n; i++)
		a[i] = i;

	std::random_shuffle(&a[0], &a[n-1]);
	Func::run(a, n, IntLessOrEqual());

	for(int i=0; i<n; i++)
		assert(a[i] == i);
}

struct BinarySearchIterative
{
	template<class T, class Tcomparer>
	static int run(T a[], int n, T t, Tcomparer &cmp)
	{
		int lo = 0;
		int hi = n-1;
		
		while(lo <= hi)
		{
			int m = (lo + hi) / 2;

			if(cmp(a[m], t))
				lo = m + 1;
			else if (cmp(t, a[m]))
				hi = m - 1;
			else
				return m;
		}
		return -1; // failure
	}
};

struct BinarySearchRecursion
{
	template<class T, class Tcomparer>
	static int search(T a[], int lo, int hi, T t, Tcomparer &cmp)
	{
		if(lo > hi) return -1; // error case

		int m = (lo + hi) / 2;
		if(cmp(a[m], t))
			return search(a, m+1, hi, t, cmp);
		else if(cmp(t, a[m]))
			return search(a, lo, m-1, t, cmp);
		else
			return m;
	}

	template<class T, class Tcomparer>
	static int run(T a[], int n, T t, Tcomparer &cmp)
	{
		return search(a, 0, n-1, t, cmp);
	}
};

template<class Func>
void test_binarySearching()
{
	const int n = 1000;

	int a[n];
	for(int i=0; i<n; i++)
		a[i] = i;

	int index;
	index = Func::run(a, n, 100, IntLess());
	assert(index == 100);
	index = Func::run(a, n, 900, IntLess());
	assert(index == 900);
	index = Func::run(a, n, 1000, IntLess());
	assert(index == -1);
}

template<class T, class TComparer>
void mergeSortedArray(T a[], int an, T b[], int bn, TComparer &cmp)
{
	int index_a = an-1;
	int index_b = bn-1;
	int index = an + bn -1;

	while(index_a >= 0 && index_b >= 0)
	{
		if(cmp(a[index_a], b[index_b]))
			a[index--] = b[index_b--];
		else
			a[index--] = a[index_a--];
	}

	while(index_b >= 0)  // !!!!!!!!!!! index_b >= 0
		a[index--] = b[index_b--];

	// if index_a > 0, values in a are already in place
}

void test_mergeSortedArray()
{
	const int an = 100; 
	const int bn = 50;

	int a[an+bn];
	for(int i=0; i<an; i++)
		a[i] = i;

	int b[bn];
	for(int i=0; i<bn; i++)
		b[i] = i * 2;

	mergeSortedArray(a, an, b, bn, IntLessOrEqual());

	for(int i=1; i<an+bn; i++)
	{
		assert(IntLessOrEqual()(a[i-1], a[i]));
	}
}

void groupAnagrams(string strs[], int n)
{
	unordered_map<string, List<string> > map;
	for(int i=0; i<n; i++)
	{
		string key = strs[i];
		sort(key.begin(), key.end()); // sort chars

		map[key].push_back(strs[i]);
	}

	int index = 0;
	unordered_map<string, List<string> >::iterator it;

	for(it = map.begin(); it!= map.end(); it++)
	{
		List<string>::Iterator it2(it->second);
		for(; it2.good(); it2.advance())
		{
			strs[index++] = it2.value();
		}
	}
}

void test_groupAnagrams()
{
	const int n = 7;
	string strs[n] = {"test", "test1", "test2", "estt", "ttse1", "sett2", "set1t"};
	groupAnagrams(strs, n);

	unordered_map<string, int> groups;
	groups["test"] = 0;
	groups["estt"] = 0;
	groups["test1"] = 1;
	groups["ttse1"] = 1;
	groups["set1t"] = 1;
	groups["test2"] = 2;
	groups["sett2"] = 2;

	vector<bool> groups_seen(3, false);
	int curr_id = -1;
	for(int i=0; i<n; i++)
	{
		const string &s = strs[i];

		assert(groups.count(s) > 0);
		const int id = groups[s];

		if(id == curr_id) // same group as prev string
			continue; 

		assert(!groups_seen[id]); // new group
		groups_seen[id] = true;
		curr_id = id;
	}
}

void test_sortingAndSearching()
{
	test_comparisonSorting<BubbleSort>();
	test_comparisonSorting<SelectionSort>();
	test_comparisonSorting<InsertionSort>();

	test_comparisonSorting<MergeSort>();
	test_comparisonSorting<QuickSort>();
	test_comparisonSorting<HeapSort>();

	test_binarySearching<BinarySearchIterative>();
	test_binarySearching<BinarySearchRecursion>();

	test_mergeSortedArray();   // 11.1
	test_groupAnagrams();      // 11.2
}