#pragma
#include "Common.h"

namespace
{
	template<class T>
	void swap(T &lhs, T &rhs)
	{
		T temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
}

struct BubbleSort
{
	// Tcomparer returns true is first argument should go before second argument
	template<class T, class Tcomparer> 
	void sort(T a[], int n, Tcomparer &cmp)	
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
	void sort(T a[], int n, Tcomparer &cmp)
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
	void sort(T a[], int n, Tcomparer &cmp)
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
	void merge(T a[], T buffer[], int start, int middle, int end, Tcomparer &cmp) // !!! left half a[start]-a[middle] and right half a[middle+1]-a[end]
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
	void sort(T a[], T buffer[], int start, int end, Tcomparer &cmp) // !!! create buffer once and pass in
	{
		if(start >= end)   // !!!!!!!! base case
			return;   

		int middle = (start + end) / 2; 

		sort(a, buffer, start, middle, cmp);
		sort(a, buffer, middle+1, end, cmp);
		merge(a, buffer, start, middle, end, cmp);
	}

	template<class T, class Tcomparer>
	void sort(T a[], int n, Tcomparer &cmp)
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
	int partition(T a[], int start, int end, int pivot, Tcomparer &cmp)
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
	void sort(T a[], int start, int end, Tcomparer &cmp)
	{
		if(start >= end) // !!!!!!!!! base case
			return;

		int pivot = start + std::rand() % (end - start + 1);  // !!!!!!!! maybe other choices
		int index = partition(a, start, end, pivot, cmp);

		sort(a, start, index-1, cmp);
		sort(a, index+1, end, cmp);
	}

	template<class T, class Tcomparer>
	void sort(T a[], int n, Tcomparer &cmp)
	{
		sort(a, 0, n-1, cmp);	
	}
};

#include "Heap.h"

struct HeapSort
{
	template<class T, class Tcomparer>
	void sort(T a[], int n, Tcomparer &cmp)
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