#include "Common.h"
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

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

bool exist(vector<vector<char> > &board, string word, int i, int j) 
{
	assert(!word.empty());

	if(i >= board.size() || j >= board[0].size() || board[i][j] != word[0]) // base cases
		return false; 

	board[i][j] = '#';
	string newWord = word.substr(1);

	if(newWord.empty() ||
	   exist(board, newWord, i, j+1) /* right */ ||
	   exist(board, newWord, i+1, j) /* down  */ ||
	   exist(board, newWord, i, j-1) /* left  */ ||
	   exist(board, newWord, i-1, j) /* up    */ )
	   return true;

	board[i][j] = word[0];
	return false;
}

bool exist(vector<vector<char> > &board, string word) 
{
	if(word.empty()) return false;

	// find starting cell
	for(int i=0; i<board.size(); i++)
		for(int j=0; j<board[0].size(); j++)
		{
			if(board[i][j] == word[0])
			{
				if(exist(board, word, i, j)) 
					return true;
			}
		}

	return false;
}

void test_exist()
{
	// passed all tests on OJ
}

int minimumTotal(vector<vector<int> > &triangle) 
{
	int n = triangle.size();
	vector<int> sums(n+1, 0); // the row below last row

	while(n >= 1)
	{
		for(int i=0; i<n; i++)
		{
			sums[i] = triangle[n-1][i] + min(sums[i], sums[i+1]); 	
		}
		n -= 1;
	}
	return sums[0];
}

void test_minimumTotal()
{
	// passed all tests on OJ
}

void generateParenthesis(vector<string> &out, string curr, int n, int l, int r)
{
	if(l == n && r == n)
	{
		out.push_back(curr);
		return;
	}

	if(l < n) generateParenthesis(out, curr+"(", n, l+1, r);
	if(l > r) generateParenthesis(out, curr+")", n, l, r+1);
}

vector<string> generateParenthesis(int n) 
{
	vector<string> out;
	generateParenthesis(out, "", n, 0, 0);
	return out;
}

void test_generateParenthesis()
{
	// all tests passed on OJ
}


void solveNQueens(vector<string> &board, int n, int row, vector<bool> &cols, vector<bool> &diags_left, vector<bool> &diags_right, vector<vector<string> > &output)
{
	if(row == n) // found valid solution
	{
		output.push_back(board);
		return;
	}

	for(int col=0; col<n; col++)
	{
		if(cols[col]) // check column attack
			continue;

		int r = col + row;   // !!!!!!! start from the top-left corner to the right
		int l = n - 1 - col + row;  // !!!!!!!!! start from the top-right corner to the left
		if(diags_left[l] || diags_right[r]) // check diagonal attack
			continue;

		// update state
		board[row][col] = 'Q';
		cols[col] = true;
		diags_left[l] = true;
		diags_right[r] = true;
		
		solveNQueens(board, n, row+1, cols, diags_left, diags_right, output);

		// revert state
		board[row][col] = '.';
		cols[col] = false;
		diags_left[l] = false;
		diags_right[r] = false;
	}
}

vector<vector<string> > solveNQueens(int n) 
{
	vector<vector<string> > output;
	if(n == 0) return output;

	vector<bool> cols(n, false);
	vector<bool> diags_left(2*n-1, false);
	vector<bool> diags_right(2*n-1, false);

	vector<string> board(n, string(n, '.'));
	solveNQueens(board, n, 0, cols, diags_left, diags_right, output);
	return output;
}

void test_solveNQueens()
{
	solveNQueens(2);
}

void test_general()
{
	// Reverse bits of an unsigned integer

	// Determine if a Sudoku puzzle is valid
	test_isValidSudoku();

	// Solve a Sudoku puzzle by filling the empty cells
	// note: backtracking
	test_solveSudoku();

	// Word Search
	test_exist();

	// Find the minimum path sum in a triangle
	test_minimumTotal();

	// Generate Parentheses
	test_generateParenthesis();

	// Find all distinct solutions to the n-queens puzzle 
	test_solveNQueens();
}