#include "Common.h"
#include <vector>
#include <assert.h>

using namespace std;

int reverseInteger(int x)
{
	bool isNeg = x < 0;
	x = abs(x);

	int result = 0;
	while(x > 0)
	{
		result = result*10 + x%10;
		x = x/10;
	}

	// check overflow 
	if(result < 0) return 10; // !!! 10,100,...reverse to 1, but nothing reverse to 10, 100,...

	return isNeg ? -1*result : result; // !!! result, not x
}

void test_reverseInteger()
{
	assert(reverseInteger(0) == 0);
	assert(reverseInteger(1) == 1);
	assert(reverseInteger(10) == 1);
	assert(reverseInteger(123) == 321);
	assert(reverseInteger(-1) == -1);
	assert(reverseInteger(-10) == -1);
	assert(reverseInteger(1000000003) == 10);
}

struct IsPalindromeIterative
{
	static bool run(int x)
	{
		if(x < 0) return false;
		if(x == 0) return true;

		// find divisor to get first digit
		int div = 1;
		while(x / div >= 10) // !!!!!!!! use >=
			div *= 10;

		while(x > 0)
		{
			// compare fist and last digit
			if(x / div != x % 10)
				return false;

			// chop off first and last digit 
			x = (x % div) / 10;
			div /= 100; // !!!!!!!! update
		}
		return true;
	}
};

struct IsPalindromeRecursion
{
	static bool isPalindrome(int x, int &y)
	{
		if(x < 0) return false;
		if(x == 0) return true;
		
		// use recursion stack to chop off first digit
		if(isPalindrome(x/10, y) && (x%10 == y%10)) // use x%10 to get first digit, y%10 to get last digit
		{
			y /= 10; // use this to chop off last digit
			return true;
		}
		else
			return false;
	}

	static bool run(int x)
	{
		return isPalindrome(x, x);
	}
};

template<class Func>
void test_isPalindrome()
{
	assert(!Func::run(-1));
	assert(Func::run(0));
	assert(Func::run(1));
	assert(!Func::run(10));
	assert(Func::run(11));
    assert(Func::run(121));
	assert(Func::run(1221));
}

bool isValidSudoku(vector<vector<char> > &board) 
{
	if(board.size() != 9) return false;
	for(int i=0; i<9; i++) 
		if(board[i].size() != 9) return false;

	vector<vector<bool> > rows(9, vector<bool>(9, false));
	vector<vector<bool> > cols(9, vector<bool>(9, false));
	vector<vector<bool> > blks(9, vector<bool>(9, false));

	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
		{
			if(board[i][j] == '.')
				continue;

			int c = board[i][j] - '1';

			int row_id = i;
			int col_id = j;
			int blk_id = (i/3)*3 + j/3;

			if(rows[row_id][c] || cols[col_id][c] || blks[blk_id][c])
				return false;

			rows[row_id][c] = cols[col_id][c] = blks[blk_id][c] = true;
		}

	return true;
}

void test_isValidSudoku()
{
	char* a[9] = 
	{
		"123456789",
		"456789123",
		"789123456",
		"234567891",
		"891234567",
		"567891234",
		"345678912",
		"678912345",
		"912345678"
	};

	vector<vector<char> > matrix(9);
	for(int i=0; i<9; i++)
		matrix[i].assign(a[i], a[i]+9);
	assert(isValidSudoku(matrix));

	for(int i=0; i<25; i++)
		matrix[rand()%9][rand()%9] = '.';
	assert(isValidSudoku(matrix));

	matrix[3][3] = '1';
	matrix[5][5] = '1';
	assert(!isValidSudoku(matrix));
}

namespace SolveSudoku
{
	bool findNextEmpty(vector<vector<char> > &board, int start_row, int start_col, int &new_row, int &new_col)
	{
		for(int i=start_row; i<9; i++)
		{
			int j = (i == start_row) ? start_col : 0;  //!!!!!!!!! must check to start at correct col
			for(; j<9; j++)
			{
				if(board[i][j] == '.')
				{
					new_row = i;
					new_col = j;
					return true;
				}
			}
		}
		return false;
	}

	vector<int> getValidCandidates(vector<vector<char> > &board, int empty_row, int empty_col)
	{
		vector<int> flags(9, false);

		int block_row = empty_row/3;
		int block_col = empty_col/3;

		for(int i=0; i<9; i++)
		{
			char value;

			value = board[empty_row][i]; // i-th value in same row
			if(value != '.') flags[value-'1'] = true;

			value = board[i][empty_col]; // i-th value in same col
			if(value != '.') flags[value-'1'] = true;

			int row = block_row*3 + i/3;
			int col = block_col*3 + i%3;
			value = board[row][col]; // i-th value in same block
			if(value != '.') flags[value-'1'] = true;
		}

		vector<int> candidates;
		for(int i=0; i<9; i++)
		{
			if(!flags[i])
				candidates.push_back(i);
		}
		return candidates;
	}

	bool solveSudoku(vector<vector<char> > &board, int start_row, int start_col)
	{
		// Use start_row and start_col avoid searching from beginning every time
		int empty_row, empty_col;
		if(!findNextEmpty(board, start_row, start_col, empty_row, empty_col))
			return true; // sudoku solved

		// It's better to enumerate only valid values, because enumerating all
		// values and then accepting or rejecting each value is more expensive here
		vector<int> values = getValidCandidates(board, empty_row, empty_col);

		for(int i=0; i<values.size(); i++)
		{
			board[empty_row][empty_col] = '1' + values[i];
			if(solveSudoku(board, empty_row, empty_col))
				return true; // sudoku solved, stop here since only need one solution
		}

		board[empty_row][empty_col] = '.'; // !!!!!!!!! must roll back to empty for backtracking
		return false; // sudoku can't be solved
	}
}

void solveSudoku(vector<vector<char> > &board) 
{
	assert(board.size() == 9);
	for(int i=0; i<9; i++)
		assert(board[i].size() == 9);

	assert(SolveSudoku::solveSudoku(board, 0, 0));
}

void test_solveSudoku()
{
	char* a[9] =
	{
		"123456789",
		"456789123",
		"789123456",
		"234567891",
		"891234567",
		"567891234",
		"345678912",
		"678912345",
		"912345678"
	};

	vector<vector<char> > board(9);
	for(int i=0; i<9; i++)
		board[i].assign(a[i], a[i]+9);

	for(int i=0; i<25; i++)
		board[rand()%9][rand()%9] = '.';

	solveSudoku(board);
	assert(isValidSudoku(board));
}

void test_general()
{
	// Reverse digits of an integer
	test_reverseInteger();

	// Reverse bits of an unsigned integer

	// Determine whether an integer is a palindrome. Do this without extra space
	test_isPalindrome<IsPalindromeIterative>();
	test_isPalindrome<IsPalindromeRecursion>();

	// Determine if a Sudoku puzzle is valid
	test_isValidSudoku();

	// Solve a Sudoku puzzle by filling the empty cells
	// note: backtracking
	test_solveSudoku();
}