#include "Common.h"
#include <vector>
#include <assert.h>

using namespace std;

int uniquePaths(int m, int n, int i, int j, vector<vector<int> > &cache)
{
	if(i>=m || j>=n) return 0; // failure case, backtracking
	if(i==m-1 && j==n-1) return 1; // base case

	if(cache[i][j] == -1) // !!!!! zero-based index
	{
		cache[i][j] = uniquePaths(m, n, i+1, j, cache) + 
			          uniquePaths(m, n, i, j+1, cache);
	}

	return cache[i][j];
}

int uniquePaths(int m, int n)
{
	vector<vector<int> > cache(m, vector<int>(n, -1));
	return uniquePaths(m, n, 0, 0, cache); // !!!!!!! zero-based index
}

void test_uniquePaths()
{
	assert(uniquePaths(1,1) == 1);
	assert(uniquePaths(2,2) == 2);
	assert(uniquePaths(3,3) == 6);
}

int uniquePathsWithObstacles(int m, int n, int i, int j, vector<vector<int> > &obstacleGrid, vector<vector<int> > &cache)
{
	if(i>=m || j>=n) return 0; // failure case, backtracking
	if(obstacleGrid[i][j] == 1) return 0; // obstacle case !!!!!!! must come before base case
	if(i==m-1 && j==n-1) return 1; // base case

	if(cache[i][j] == -1)
	{
		cache[i][j] = uniquePathsWithObstacles(m, n, i+1, j, obstacleGrid, cache) + 
			          uniquePathsWithObstacles(m, n, i, j+1, obstacleGrid, cache);
	}

	return cache[i][j];
}

int uniquePathsWithObstacles(vector<vector<int> > &obstacleGrid)
{
	assert(obstacleGrid.size() != 0 && obstacleGrid[0].size() != 0);

	int m = obstacleGrid.size();
	int n = obstacleGrid[0].size();

	vector<vector<int> > cache(m, vector<int>(n, -1));
	return uniquePathsWithObstacles(m, n, 0, 0, obstacleGrid, cache);
}

void test_uniquePathsWithObstacles()
{
	vector<vector<int> > obstacleGrid_a(1, vector<int>(1, 0));
	obstacleGrid_a[0][0] = 1;
	assert(uniquePathsWithObstacles(obstacleGrid_a) == 0);

	vector<vector<int> > obstacleGrid_b(3, vector<int>(3, 0));
	obstacleGrid_b[1][1] = 1;
	assert(uniquePathsWithObstacles(obstacleGrid_b) == 2);
}

int climbStairs(int n, int start, vector<int> &cache)
{
	if(start > n)   
		return 0;

	if(start == n) // base case !!!!!! n is the last step
		return 1;
	
	if(cache[start] < 0)
	{
		cache[start] = climbStairs(n, start + 1, cache) + 
			climbStairs(n, start + 2, cache);
	}

	return cache[start];
}

int climbStairs(int n) 
{
	vector<int> cache(n+1, -1);
	return climbStairs(n, 0, cache);
}

void test_climbStairs()
{
	climbStairs(1);
}

void test_dynamicDP()
{
	// Count unique paths in a m*n grid from (1,1) to (N,N)
	// note: backtracking with DP
	test_uniquePaths();

	// Count unique paths in a m*n grid from (1,1) to (N,N) with obstacles
	// note: backtracking with DP
	test_uniquePathsWithObstacles();

	// Count unique ways to climb stairs to reach top
	test_climbStairs();


}