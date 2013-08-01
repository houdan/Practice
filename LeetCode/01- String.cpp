#include <string>
#include <vector>
#include <assert.h>

using namespace std;

// time O(N^2), space O(N^2)
struct LongestPalindromeDPRecursion
{
	static bool isPalindrome(const string &s, int i, int j, vector<vector<int> > &cache)
	{
		assert(i <= j);

		if(cache[i][j] < 0) 
		{
			if(i == j || i == j-1) // base cases: "a" "bb"
			{
				cache[i][j] = (s[i] == s[j]) ? 1 : 0;
			}
			else
			{
				cache[i][j] = ((s[i] == s[j]) && isPalindrome(s, i+1, j-1, cache)) ? 1 : 0;
			}
		}
		return cache[i][j] > 0;
	}

	static string run(const string &s)
	{
		if(s.empty()) return "";

		// init cache with -1
		vector<vector<int> > cache(s.length());
		for(int i=0; i<s.length(); i++)
			cache[i].resize(s.length(), -1);

		// try different substring, from large to small
		int len = s.length();
		while(len > 0)
		{
			for(int i=0; i<s.length()-len+1; i++)
			{
				if(isPalindrome(s, i, i+len-1, cache))
					return s.substr(i, len);
			}
			len--;
		}

		assert(0); // this should never happen
	}
};

// time O(N^2), space O(N^2)
struct LongestPalindromeDPIterative
{
	static string run(const string &s)
	{
		if(s.empty()) return "";

		int longestStart = 0;
		int longestLength = 1;

		// init cache with false: because iterate from base case
		vector<vector<bool> > cache(s.length());
		for(int i=0; i<s.length(); i++)
			cache[i].resize(s.length(), false);

		// mark base case, size=1
		for(int i=0; i<s.length(); i++)
			cache[i][i] = true;

		// mark base case, size=2
		for(int i=0; i<s.length()-1; i++)
		{
			if(s[i] == s[i+1])
			{
				cache[i][i+1] = true;
				longestStart = i;
				longestLength = 2;
			}
		}

		int len = 3;
		while(len<=s.length())
		{
			for(int i=0; i<s.length()-len+1; i++)
			{
				int j = i+len-1;
				if(s[i]==s[j] && cache[i+1][j-1])
				{
					cache[i][j] = true;
					longestStart = i;
					longestLength = len;
				}
			}
			len++;
		}

		return s.substr(longestStart, longestLength);
	}
};

// time O(N^2), space O(1)
struct LongestPalindromeExpand
{
	static string expand(const string &s, int i, int j)
	{
		assert(i <= j);

		while(i >= 0 && j < s.length() && s[i] == s[j])
		{
			i--;
			j++;
		}

		return s.substr(i+1, j-i-1); // size = (j-1)-(i+1)+1
	}

	static string run(const string &s)
	{
		if(s.empty()) return "";
	
		string longest = s.substr(0, 1);
		for(int i=0; i<s.length()-1; i++)  // !!!!!! only move to s.length()-1
		{
			string p1 = expand(s, i, i);
			if(p1.length() > longest.length())
				longest = p1;

			string p2 = expand(s, i, i+1);
			if(p2.length() > longest.length())
				longest = p2;
		}

		return longest;
	}
};

// Given a string S, find the longest palindromic substring in S
template<class Func>
void test_longestPalindrome()
{
	assert("a" == Func::run("abcde"));
	assert("bcb" == Func::run("abcbe"));
	assert("jklmnnmlkj" == Func::run("abcdefghijklmnnmlkjabcde"));

	string s(100, 'a');
	assert(s == Func::run(s));
}

void test_string()
{
	// Given a string S, find the longest palindromic substring in S
	test_longestPalindrome<LongestPalindromeDPRecursion>();  // more efficient
	test_longestPalindrome<LongestPalindromeDPIterative>();
	test_longestPalindrome<LongestPalindromeExpand>();

	// how about longest common substring??
	// how about longest common subsequence??
	// how about longest Palindromic subsequence??
}