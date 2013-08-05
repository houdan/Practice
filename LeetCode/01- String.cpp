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


struct IsMatchRegexWithBT
{
	// backtracking here only needs to find one valid solution
	static bool run(const char *s, const char *p) 
	{
		if(*p == 0) return *s == 0; // base case
		// note: if *s==0 but *p!=0, need to wait to see *(p+1) is * to cancel *p

		// see if *p matches *s: 'a' - 'a', or 'a' - '.'
		if(*p == *s || (*p == '.' && *s != 0))
		{
			return *(p+1) != '*' ? run(s+1, p+1) /* both move to next */: 
				run(s+1, p) /* repeat more times */ || run(s, p+2) /* repeat zero time */;
		}
		else // see if *p can be cancelled by '*': 'a' - 'b*', or '\0' - '.*' 
		{
			return *(p+1) == '*' && run(s, p+2);
		}
	}
};

struct IsMatchRegexWithDP
{
	static bool run(const char *s, const char *p) 
	{
		int s_len = strlen(s);
		int p_len = strlen(p);

		// cache(i,j) entry means if p's first i chars match s' first j chars
		vector<vector<bool> > cache(p_len+1, vector<bool>(s_len+1, false));

		// init
		cache[0][0] = true;
		for(int i=1; i<=s_len; i++)
		{
			cache[0][i] = false;
		}
		for(int i=1; i<=p_len; i++)
		{
			if(p[i-1] == '*') 
			{
				assert(i > 1); // '*' can never be at p[0]
				cache[i][0] = cache[i-2][0]; // '*' can always cancel prev char
			}
			else
				cache[i][0] = false;
		}

		// since it scans row by row, DP cache could just store two rows: last and current
		for(int i=1; i<=p_len; i++)
			for(int j=1; j<=s_len; j++)
			{
				if(p[i-1] == '*') // special case: '*' - 'A'
				{
					assert(i > 1); // '*' can never be at p[0]
					cache[i][j] = (cache[i-1][j] || cache[i-2][j]) /* p=[__a|*__] s=[__a|__] or p=[__ab|*__] s=[__a|__] */ ||
						(cache[i][j-1] && (p[i-2] == s[j-1] || p[i-2] == '.') /* p=[__a|*__] s=[__|a__] or p=[__.|*__] s=[__|a__] */ );
				}
				else if(p[i-1] == s[j-1] || p[i-1] == '.') // match case: 'A' - 'A' or '.' - 'A'
				{
					cache[i][j] = cache[i-1][j-1];
				}
				else // failure case: 'A' - 'B'
				{
					cache[i][j] = false;
				}
			}

		return cache[p_len][s_len];
	}
};

template<class Func>
void test_isMatchRegex()
{
	assert(!Func::run("aa", "a"));   
	assert(Func::run("aa", "aa")); 
	assert(!Func::run("aaa", "aa")); 
	assert(Func::run("aa", "a*")); 
	assert(Func::run("aa", ".*")); 
	assert(Func::run("ab", ".*")); 
	assert(Func::run("aab", "c*a*b")); 
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

	// Implement regular expression matching (full string)
	// note: "." should not match to "\0" terminator
	//       "*" should be part of prev char, e.g. "a*" or ".*"
	test_isMatchRegex<IsMatchRegexWithBT>(); // backtracking
	test_isMatchRegex<IsMatchRegexWithDP>(); // dynamic programming

	// how about matching with partial string??


	
}