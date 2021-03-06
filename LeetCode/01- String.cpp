#include <string>
#include <vector>
#include <unordered_map>
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

string minWindow(string S, string T) 
{
	int t_count[256] = {0}; // char count in T
	int w_count[256] = {0}; // char count in window

	for(int i=0; i<T.length(); i++)
		t_count[T[i]]++;

	int t_count_total = T.length();
	int w_count_found = 0;

	int w_start = 0, w_end = 0; // current window
	int w_start_min = 0, w_length_min = INT_MAX; // minimum window 

	for(; w_end < S.length(); w_end++) // move w_end to right by 1
	{
		// advance w_end to meet constraint
		char c = S[w_end];
		if(t_count[c] == 0) continue; // not in T
	
		w_count[c]++; 
	    if(t_count[c] >= w_count[c]) w_count_found++; // only increment if c helps meet constraints
		
		if(w_count_found == t_count_total) // !!!! this will always be entered after first valid window is found 
		{
			// advance w_start to right as far as possible while maintaining constrants
			for(; w_start <= w_end; w_start++)  // !!!!!! this could be optimized to directly jump to next possible position (indices of chars in S that exists in T)
			{
				char d = S[w_start];
				if(t_count[d] == 0) continue;  // not in T
					
				if(t_count[d] < w_count[d])
					w_count[d]--;
				else
					break; // if advance past this, will break constraints 
			}

			// update minimum window
			int w_length = w_end - w_start + 1;
			if(w_length < w_length_min)
			{
				w_start_min = w_start;
				w_length_min = w_length;
			}
		}
	}

	if(w_length_min != INT_MAX)
		return S.substr(w_start_min, w_length_min);
	else
		return "";
}

void test_minWindow()
{
	assert(minWindow("cabwefgewcwaefgcf", "cae") == "cwae");
}

int numDistinct(string S, string T) 
{
	vector<int> cache(T.length()+1, 0);
	cache[0] = 1; // !!!! this is always unchanged

	for(int i = 0; i < S.length(); i++)
	{
		// need to move backwards 
		// cache[i][j] = cache[i-1][j] + (S[i-1]==T[j-1]) * cache[i-1][j-1] 
		for(int j = T.length()-1; j >= 0; j--)
		{
			if(T[j] == S[i])
				cache[j+1] += cache[j];
		}
	}

	return cache.back();
}

void test_numDistinct()
{
	// all tests passed on OJ
}

int lengthOfLongestSubstring(string s) 
{
	bool flag[26];
	for(int i=0; i<26; i++)
		flag[i] = false;

	int maxLen = 0;
	int i = 0;
	int j = 0;
	while(i < s.length()) 
	{
		while(j < s.length()) // move window end position as far as it can
		{
			int id = s[j] - 'a';
			if(flag[id])
				break;
				
			flag[id] = true;
			j = j + 1;
		}

		// update max window
		maxLen = std::max(maxLen, j - i); 

		if (j == s.length())  // !!!!!!!!! no need to move window start if window end already reaches string end
			break;

		// move window start position by 1  
		int id = s[i] - 'a';
		flag[id] = false;
		i = i + 1;
	}

	return maxLen;
}

void test_lengthOfLongestSubstring()
{
	lengthOfLongestSubstring("abcd");
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

	// Minimum Window Substring
	test_minWindow();

	// Distinct Subsequences
	test_numDistinct();

	// Longest Substring Without Repeating Characters
	test_lengthOfLongestSubstring();
}