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

int maxProfit(vector<int> &prices, int &buy, int &sell) 
{
	// DP simplified - only remember most recent entry
	int minIndex = 0;
	int maxProfit = 0;
	buy = sell = 0;

	for(int i=1; i<prices.size(); i++)
	{
		int price = prices[i];
		if(price < prices[minIndex])
			minIndex = i;

		int profit = price - prices[minIndex];
		if(profit > maxProfit)
		{
			sell = i;
			buy = minIndex;
			maxProfit = profit;
		}
	}

	return maxProfit;
}

int maxProfit(vector<int> &prices)
{
	int buy, sell;
	return maxProfit(prices, buy, sell);
}

void test_maxProfit()
{
	// passed all tests on OJ
}

int maxProfit2(vector<int> &prices) 
{
	int profit = 0;
	for(int i=1; i<prices.size(); i++)
	{
		int delta = prices[i] - prices[i-1];
		if(delta > 0) profit += delta;
	}

	return profit;
}

void test_maxProfit2() 
{
	// passed all tests on OJ
}

int maxProfit3(vector<int> &prices) 
{
	const int n = prices.size();
	if(n == 0) return 0;

	// DP for max profit of each segment
	vector<int> pastMaxProfit(n, 0);
	vector<int> postMaxProfit(n, 0);  // !!! in fact this is not required, could just use one variable

	int minPrice = INT_MAX;
	for(int i=0; i< n; i++)  // !!! these two scans could be merged into one, see test_multiplication() 
	{
		minPrice = min(minPrice, prices[i]);
		if(i>0)
			pastMaxProfit[i] = max(pastMaxProfit[i-1], prices[i] - minPrice);
	}

	int maxProfit = INT_MIN;
	int maxPrice = INT_MIN;
	for(int i=n-1; i>=0; i--)
	{
		maxPrice = max(maxPrice, prices[i]);
		if(i<n-1)
			postMaxProfit[i] = max(postMaxProfit[i+1], maxPrice - prices[i]);
		maxProfit = max(maxProfit, pastMaxProfit[i] + postMaxProfit[i]);
	}

	return maxProfit;
}

void test_maxProfit3()
{
	// passed all tests on OJ
}

vector<int> multiplication(int a[], int n)
{
	vector<int> out(n, 1);

	int left = 1, right = 1;
	for(int i=0; i<n; i++)
	{
		out[i] *= left; // multiply with the product before i
		out[n-1-i] *= right; // multiply with the product after n-1-i

		left *= a[i];
		right *= a[n-1-i];
	}

	return out;
}

void test_multiplication()
{
	const int count = 5;
	int a[] = {1,2,3,4,5};
	
	vector<int> out = multiplication(a, 5);
	
	int b[] = {120,60,40,30,24};
	for(int i=0; i<count; i++)
		assert(b[i] == out[i]);
}

// the insert pos: a[pos-1].start < val <= intervals[pos].start
// when val is smaller than a[0], it returns 0
// when interval array is empty, it returns 0
int searchInsert(int a[], int n, int t) 
{
	int lo = 0;
	int hi = n-1;
	while(lo <= hi)
	{
		int mi = (lo + hi) / 2;
		if(a[mi] == t)
			return mi;
		else if (a[mi] < t)
			lo = mi + 1;
		else
			hi = mi - 1;
	}

	// when t is not in a[], it iterates until lo=hi=mi,
	// if a[mi] < t, lo = mi+1  
	// if a[mi] > t; lo = lo
	return lo; 
}

void test_searchInsert()
{
	// passed all tests on OJ
}

// the pos of val: intervals[pos].start <= val < intervals[pos+1].start
// when val is smaller than intervals[0].start, it returns -1
// when interval array is empty, it returns -1
int searchPosition(vector<Interval> &intervals, int val)
{
	int lo = 0;
	int hi = intervals.size() - 1;
	while(lo <= hi)
	{
		int mi = (lo + hi) / 2;
		if(intervals[mi].start == val)
			return mi;
		else if (intervals[mi].start < val)
			lo = mi + 1;
		else
			hi = mi - 1;
	}

	// when val doesn't match all interval starts, it iterates until lo=hi=mi,
	// if a[mi] < val, hi = hi  
	// if a[mi] > val; hi = mi -1;
	return hi;  
}

vector<Interval> insertInterval(vector<Interval> &intervals, Interval newInterval) 
{
	int startPos = searchPosition(intervals, newInterval.start);
	int endPos = searchPosition(intervals, newInterval.end);

	if(startPos >= 0 && intervals[startPos].end >= newInterval.start) 
	{
		newInterval.start = intervals[startPos].start; // newInterval.start falls within the interval at startPos, so extend start
	}
	else  
		startPos++; // newInterval.start falls behind the interval at startPos, so advance startPos    

	if(endPos >= 0 && intervals[endPos].end > newInterval.end)  
	{
		newInterval.end = intervals[endPos].end; // newInterval.end falls within the interval at endPos, so extend end
	}

	if(endPos >= startPos)
		intervals.erase(intervals.begin() + startPos, intervals.begin() + endPos + 1);

	intervals.insert(intervals.begin() + startPos, newInterval);
	return intervals;
}

void test_insertInterval()
{
	// passed all tests on OJ
}

int removeDuplicates(int a[], int n) 
{
	if(n < 2) return n;

	int len = 1; // len of array without duplicates, also points to next insert position
	for(int i=1; i<n; i++)
	{
		if(a[i] == a[i-1])
			continue; // duplicate

		if(len != i)
			a[len] = a[i]; // need to copy

		len += 1;
	}

	return len;
}

void test_removeDuplicates()
{
	// passed all tests on OJ
}

int removeDuplicates2(int a[], int n) 
{
	if(n < 2) return n;

	int len = 1; // len of array without duplicates, also points to next insert position
	int last = a[0]; // last distinct value
	int last_count = 1;
	for(int i=1; i<n; i++)
	{
		if(a[i] == last && last_count++ >= 2)
			continue; // duplicate more than twice

		if(a[i] != last)
		{
			last = a[i];  // reset
			last_count = 1;
		}

		if(len != i)
			a[len] = a[i]; // need to copy

		len += 1;
	}

	return len;
}

void test_removeDuplicates2()
{
	// passed all tests on OJ
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

	// Best Time to Buy and Sell Stock (single transaction)
	test_maxProfit(); 

	// Best Time to Buy and Sell Stock (multiple transactions - non-overlapped)
	test_maxProfit2(); 

	// Best Time to Buy and Sell Stock (two transactions - non-overlapped)
	test_maxProfit3(); 

	// Find all multiplications of all numbers without a[i]
	test_multiplication();

	// Search Insert Position
	test_searchInsert();

	// Insert Interval
	test_insertInterval();

	// Remove Duplicates from Sorted Array
	test_removeDuplicates();

	// Remove Duplicates from Sorted Array (Allow twice)
	test_removeDuplicates2();
}