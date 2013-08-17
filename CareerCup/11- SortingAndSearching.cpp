#include "Common.h"
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

using namespace std;

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
	test_binarySearching<BinarySearchIterative>();
	test_binarySearching<BinarySearchRecursion>();

	test_mergeSortedArray();   // 11.1
	test_groupAnagrams();      // 11.2
}