#pragma once
#include "Common.h"
#include <algorithm> // swap
#include <math.h> // floor
#include <vector>

template<class T, class TComparer>
class Heap
{
public:
	void insert(const T &val)
	{
		buffer.push_back(val);

		int n = buffer.size();
		siftUp(buffer.data(), n, n-1, TComparer());
	}

	T pop()
	{
		if(empty()) std::abort();

		T val = buffer.front();
		std::swap(buffer.front(), buffer.back());

		int n = buffer.size();
		buffer.resize(n-1);
		siftDown(buffer.data(), n-1, 0, TComparer());

		return val;
	}

	T peek()
	{
		if(empty()) std::abort();
		return buffer.front();
	}

	bool empty()
	{
		return buffer.size() == 0;
	}

private:
	std::vector<T> buffer;
};

template<class T, class TComparer>
void siftUp(T a[], int size, int start, TComparer &cmp)
{
	int i = start;
	while(true)
	{
		int parent = std::floor(float(i-1)/2); 
		if(parent < 0) return; // no parent anymore

		if(cmp(a[parent], a[i])) return; // no need to siftup anymore
		
		std::swap(a[parent], a[i]);
		i = parent;
	}
}

template<class T, class TComparer>
void siftDown(T a[], int size, int start, TComparer &cmp)
{
	int i = start;
	while(true) 
	{
		int child = 2*i+1;
		if(child >= size) return; // no children anymore

		int swap = i;

		// check left child
		if(cmp(a[child], a[swap]))
			swap = child;

		// check right child
		child += 1;
		if(child < size && cmp(a[child], a[swap]))
			swap = child;

		if(swap == i) return; // no need to siftdown anymore

		// swap and continue siftdown
		std::swap(a[i], a[swap]);
		i = swap;
	}
}

template<class T, class TComparer>
void heapify(T a[], int size, TComparer &cmp, bool bottomUp=true)
{
	if(bottomUp)
	{
		// the parent of last leaf is a[size-1]
		int i = floor(float(size-2)/2); 
		while(i >= 0)
		{
			siftDown(a, size, i, cmp);
			i -= 1;
		}
	}
	else
	{
		int i = 0;
		while(i < size)
		{
			siftUp(a, size, i, cmp);
			i += 1;
		}
	}
}