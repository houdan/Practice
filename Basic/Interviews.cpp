#include "Common.h"
#include <unordered_map>
#include <assert.h>

using namespace std;

namespace A9
{
	struct Node
	{
		int data;
		Node* parent;
	};

	int computeDistance(Node* n1, Node* n2)
	{
		unordered_map<Node*, int> n1_ancestors;
		int distance = 0;
		while(n1)
		{
			n1_ancestors[n1] = distance;
			distance++;
			n1 = n1->parent;  // !!!!!!!!!!!!!!!!
		}

		distance = 0;
		while(n2)
		{
			if(n1_ancestors.count(n2) > 0)
				return n1_ancestors[n2] + distance;
			distance++;
			n2 = n2->parent;
		}


		return -1;
	}

	void test_computeDistance()
	{
		// Complete Binary Tree
		const int count = 10;
		Node nodes[count];
		for(int i=0; i<count; i++)
		{
			nodes[i].data = i;
			nodes[i].parent = (i>0) ? &nodes[(i-1)/2] : NULL;
		}

		assert(1 == computeDistance(&nodes[0], &nodes[2]));
		assert(4 == computeDistance(&nodes[7], &nodes[9]));
		assert(5 == computeDistance(&nodes[8], &nodes[6]));
		assert(0 == computeDistance(&nodes[4], &nodes[4]));
	}
};

namespace Netflix
{
	void moveZeros(int list[], int size)
	{
		int index = -1;
		for(int i=0; i<size; i++)
		{
			int c = list[i];
			if(c == 0)
			{
				if(index < 0)
					index = i;
			}
			else
			{
				if(index >= 0 && index < i /* this must be true */)
				{
					list[index++] = list[i];
					list[i] = 0;
				}
			}
		}
	}

	void moveZeros2(int list[], int size)
	{
		int index = 0;
		for(int i=0; i<size; i++)
		{
			if(list[i] == 0)
				continue;

			if(index < i)
			{
				list[index] = list[i];
				list[i] = 0;
			}
			index++;
		}
	}

	void test_moveZeros()
	{
		int a[10] = {0,1,2,0,0,3,4,0,5,6};
		int b[10] = {0,1,2,0,0,3,4,0,5,6};
		int c[10] = {1,2,3,4,5,6,0,0,0,0};

		moveZeros(a, 10);
		for(int i=0; i<10; i++)
			assert(a[i] == c[i]);

		moveZeros(b, 10);
		for(int i=0; i<10; i++)
			assert(b[i] == c[i]);
	}
};

namespace Ooyala
{
	template<int M, int N>
	void getDiagonals(int a[M][N], Queue<List<int> > &out)
	{
		// Get diagnals starting from first row
		for(int i=0; i<N; i++)
		{
			int row = 0;
			int col = i;

			List<int> diag;	
			while(row<M && col>=0)
			{
				diag.push_back(a[row++][col--]);
			}

			out.enqueue(diag);
		}

		// Get diagnals starting from last col
		for(int i=1; i<M; i++)
		{
			int row = i;
			int col = N-1;

			List<int> diag;	
			while(row<M && col>=0)
			{
				diag.push_back(a[row++][col--]);
			}

			out.enqueue(diag);
		}
	}

	void test_getDiagonals()
	{
		int a[4][3] = 
		{
			{0, 1, 2},
			{3, 4, 5},
			{6, 7, 8},
			{9, 10, 11}
		};

		Queue<List<int> > out;
		getDiagonals<4,3>(a, out);
		
		int d[6][3] = 
		{
			// -1 is just for padding
			{0, -1, -1},
			{1, 3, -1},
			{2, 4, 6},
			{5, 7, 9},
			{8, 10, -1},
			{11, -1, -1}
		};

		int i = 0;
		while(!out.empty())
		{
			int j = 0;
			List<int> diag = out.dequeue();
			List<int>::Iterator it(diag);
			for(; it.good(); it.advance())
			{
				assert(d[i][j++] == it.value());
			}
			i++;
		}
		assert(i == 6);
	}
}

void test_interviews()
{
	A9::test_computeDistance();
	Netflix::test_moveZeros();
	Ooyala::test_getDiagonals();
}