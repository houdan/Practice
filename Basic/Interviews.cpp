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

void test_interviews()
{
	A9::test_computeDistance();
	Netflix::test_moveZeros();
}