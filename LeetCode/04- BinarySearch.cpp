#include <vector>
#include <assert.h>

using namespace std;

int searchRotatedSortedArray(int a[], int n, int k)
{
	int lo = 0;
	int hi = n-1;

	while(lo <= hi)
	{
		int mi = (lo + hi) / 2;

		if(a[mi] == k) return mi;

		// !!!! test upper half first, because when n=2, mi==lo, one need to check a[lo] <= a[hi]
		if(a[mi] < a[hi]) // upper half is sorted  
		{
			if(a[mi] < k && a[hi] >= k)
				lo = mi + 1;
			else
				hi = mi - 1;
		}
		else // upper half is sorted
		{
			if(a[mi] > k && a[lo] <= k) // in range of lower half
				hi = mi - 1;
			else 
				lo = mi + 1;
		}
	}
	return -1;
}

void test_searchRotatedSortedArray()
{
	const int count = 10;
	int a[count] = {6,7,8,9,0,1,2,3,4,5};

	assert(searchRotatedSortedArray(a, count, 10) == -1);
	assert(searchRotatedSortedArray(a, count, 7) == 1);
	assert(searchRotatedSortedArray(a, count, 5) == 9);
}

// same as finding the index of minimum value 
// minimum value always exist in the half where left_end > right_end
int findSortedArrayRotation(int a[], int n)
{
	int lo = 0;
	int hi = n-1;
	while(a[lo] > a[hi])
	{
		int mi = (lo + hi) / 2;

		// !!!! test upper half first, because can't make decision when n=2, mi==lo
		if(a[mi] > a[hi])  
			lo = mi + 1;
		else
			hi = mi;
	}

	return lo;
}

void test_findSortedArrayRotation()
{
	const int count = 10;
	int a[count] = {6,7,8,9,0,1,2,3,4,5};
	assert(findSortedArrayRotation(a, count) == 4);

	int b[2] = {1,0};
	assert(findSortedArrayRotation(b, 2) == 1);
}

bool searchMatrix(vector<vector<int> > &matrix, int k) 
{
	if(matrix.size() == 0 || matrix[0].size() == 0) 
		return false;

	int m = matrix.size();
	int n = matrix[0].size();

	if(matrix[0][0] > k || matrix[m-1][n-1] < k)
		return false;

	// first find the row i that contains k
	int row = -1, lo = 0, hi = m-1;
	while(lo <= hi)
	{
		int mi = (lo + hi) / 2;
		int val = matrix[mi][0];

		if(val == k) return true;
		
		if(val < k)
		{
			// find desired row if its last row, or next row has first value > k
			if(mi == m-1 || k < matrix[mi+1][0]) // !!!!!!!!!
			{	
				row = mi;
				break;
			}
			else
				lo = mi + 1;
		}
		else
			hi = mi - 1;
	}
	assert(row > -1);

	// then try to find k in row i
	lo = 0, hi = n-1;
	while(lo <= hi)
	{
		int mi = (lo + hi)/2;
		int val = matrix[row][mi];

		if(val == k) return true;

		if(val < k)
			lo = mi + 1;
		else
			hi = mi - 1;
	}
	return false;
}

void test_searchMatrix()
{
	const int row = 3;
	const int col = 5;

	int a[row][col] = 
	{
		{1,2,3,4,5},
		{6,7,8,9,10},
		{11,12,13,14,15}
	};

	vector<vector<int> > m(row);
	for(int i=0; i<row; i++)
	{
		m[i].assign(a[i], a[i]+col);  // !!!!!!!!! assign only copy up to the value before last
	}

	assert(!searchMatrix(m, 0));
	assert(!searchMatrix(m, 16));
	assert(searchMatrix(m, 5));
	assert(searchMatrix(m, 6));
	assert(searchMatrix(m, 13));
}

void test_binarySearch()
{
	// Search in sorted array - done

	// Search and Count occurance in Sorted Array (with duplicates)
	// note: runtime O(lg(n) + k)

	// Search in Rotated Sorted Array
	test_searchRotatedSortedArray();

	// Search in Rotated Sorted Array (with duplicates) - ??????
	// note: with duplicates, can't decide the half is sorted by values of two ends

	// Finding the rotated amount of sorted array:
	// note: same as find the min value index
    test_findSortedArrayRotation();

	// Searching a 2D Sorted Matrix (with both row and column sorted)
	// note: runtime O(n), stepwise with each step eliminating either a row or a column.

	// Searching a 2D Sorted Matrix (with row sorted, each row greater than previous row)
	test_searchMatrix();
}