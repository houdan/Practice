#include "Common.h"
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <sstream>

using namespace std;

// This assumes ASCII string
bool hasUniqueChars(const string &s)
{
	const int len = s.length();
	if(len > 256)
		return false;

	vector<bool> chars(256, false);
	for(int i=0; i<len; i++)
	{
		int c = s.data()[i]; //!!!!
		if(chars[c])
			return false;
		chars[c] = true;
	}
	return true;
}

void test_hasUniqueChars()
{
	assert(hasUniqueChars("abcdef") == true);
	assert(hasUniqueChars("aabcdef") == false);
}

void reverse(char *str)
{
	char *end = str;
	while(*end)
		end++;
	end--; //!!!!!!

	char tmp;
	while(str < end) //!!!!!
	{
		tmp = *str;
		*str++ = *end;
		*end-- = tmp;
	}
}

void test_reverse()
{
	// can't just use char* !!! 
	char a[10] = "abcde";
	char b[10] = "edcba";

	reverse(a);
	assert(strcmp(a,b) == 0);
}

bool isPermutation(const string &a, const string &b)
{
	if(a.length() != b.length()) //!!!!!!!!
		return false;

	vector<int> count(256, 0);
	for(int i=0; i<a.length(); i++)
	{
		int c = a.data()[i];
		count[c]++;
	}

	for(int i=0; i<b.length(); i++)
	{
		int c = b.data()[i];
		if(--count[c]<0) //!!!!!!!!!!!!!!
			return false;
	}
	return true;
}

void test_isPermutation()
{
	assert(isPermutation("abcde", "acebd") == true);
	assert(isPermutation("abcde", "acedd") == false);
}

void replaceSpace(char* s, int length)
{
	int count = 0;
	for(int i=0; i<length; i++) //!!!!!!!! don't advance pointer s
	{
		if(s[i] == ' ')
			count++;
	}

	char* newEnd = s + length + count * 2; 
	char* oldEnd = s + length; 

	*(newEnd+1) = '\0'; //!!!!!!!!!!!!

	while(oldEnd >= s)
	{
		if(*oldEnd == ' ')
		{
			*newEnd-- = '0';
			*newEnd-- = '2';
			*newEnd-- = '%';
		}
		else
		{
			*newEnd-- = *oldEnd;
		}

		oldEnd--; //!!!!!!!!!
	}
}

void test_replaceSpace()
{
	char a[100] = "test replace space ";
	char b[100] = "test%20replace%20space%20";

	replaceSpace(a, strlen(a));
	assert(strcmp(a,b) == 0);
}

int cmputecmpressionLength(const string &s)
{
	int len = 0;
	stringstream tmp;

	int curr = 0;
	while(curr < s.length())
	{
		char c = s.data()[curr];

		int next = curr + 1;
		while(s.data()[next] == c)
			next++;
		
		int count = next - curr;
		
    	tmp << count;
		len += (1 + tmp.str().length());

		tmp.str("");
		tmp.clear(); // !!!!!!

		curr = next; // !!!!!!!!!
	}

	return len;
}

// This version works on stringstream for concatenating chars and ints. 
// Alternatively, one could create char array based on cmpression size,
// and convert int to char array based on modulo (better to work backwards)
string basicStringcmpression(const string &s)
{
	int old_len = s.length();
	int new_len = cmputecmpressionLength(s); // !!!!!!!!
	if(new_len >= old_len)
		return s;

	stringstream new_s;
	int curr = 0;
	while(curr < old_len)
	{
		char c = s.data()[curr];

		int next = curr + 1;
		while(s.data()[next] == c)
			next++;
		
		int count = next - curr;
    	new_s << c << count;

		curr = next; // !!!!!!!!!
	}

	return new_s.str();
}

void test_basicStringcmpression()
{
	string a = basicStringcmpression("abbcccdddd");
	assert(strcmp(a.c_str(), "a1b2c3d4") == 0); 

    string b = basicStringcmpression("abbccdd");
	assert(strcmp(b.c_str(), "abbccdd") == 0); 
}

// C++ doesn't allow arbitray two-dimensional array passed as argument,
// so use template here for simplicify
template<int N> 
void rotateMatrix(int m[N][N])
{
	for(int i=0; i<N/2; i++) //!!!!!!!!!!
	{
		int start = i;
		int end = N-1-i;

		for(int j=start; j<end; j++)
		{
			int offset = j - start;

			int top = m[start][j];
			// Left -> Top
			m[start][j] = m[end-offset][start];
			// Bottom -> Left
			m[end-offset][start] = m[end][end-offset];
			// Right -> Bottom
			m[end][end-offset] = m[j][end];
			// Bottom -> Top
			m[j][end] = top;
		}
	}
}

void test_rotateMatrix()
{
	int a[4][4] = 
	{
		{1,   2,  3,  4},
		{5,   6,  7,  8},
		{9,  10, 11, 12},
		{13, 14, 15, 16},
	};

	int b[4][4] = 
	{
		{13,   9,  5,  1},
		{14,  10,  6,  2},
		{15,  11,  7,  3},
		{16,  12,  8,  4},
	};

	rotateMatrix<4>(a);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
		{
			assert(a[i][j] == b[i][j]);
		}
}

template<int M, int N>
void setZeros(int m[M][N])
{
	vector<bool> rows(M, false), cols(N, false);
	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++)
		{
			if(m[i][j] == 0)
			{
				rows[i] = true;
				cols[j] = true;
			}
		}

	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++)
		{
			if(rows[i] || cols[j]) // !!!!!!!!!!
				m[i][j] = 0;
		}
}

void test_setZeros()
{
	int a[4][4] = 
	{
		{1,   0,  3,  4},
		{5,   6,  7,  8},
		{9,  10,  0, 12},
		{13, 14, 15, 16},
	};

	int b[4][4] = 
	{
		{0,   0,  0,  0},
		{5,   0,  0,  8},
		{0,   0,  0,  0},
		{13,  0,  0,  16},
	};

	setZeros<4>(a);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
		{
			assert(a[i][j] == b[i][j]);
		}
}

void test_arraysAndStrings()
{
	test_hasUniqueChars();
	test_reverse();
	test_isPermutation();
	test_replaceSpace();
	test_basicStringcmpression();
	test_rotateMatrix();
	test_setZeros();
}

