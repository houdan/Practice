#include "Common.h"
#include <iostream>
#include <assert.h>

namespace
{
	ListNode* buildLinkedList(int a[], int size)
	{
		ListNode* head = NULL;
		for(int i=size-1; i>=0; i--)
		{
			ListNode* node = new ListNode(a[i]);
			node->next = head;
			head = node;
		}
		return head;
	}

	void deleteLinkedList(ListNode* head)
	{
		while(head)
		{
			ListNode* next = head->next;
			delete head;
			head = next;
		}
	}

	bool compareLinkedList(ListNode* head1, ListNode* head2)
	{
		if(head1 == NULL && head2 == NULL)
			return true;

		if(!(head1 && head2) || head1->val != head2->val)
			return false;

		return compareLinkedList(head1->next, head2->next);
	}
}

struct ReverseIterative
{
	static ListNode* run(ListNode* head)
	{
		ListNode* newHead = NULL;
		while(head)
		{
			ListNode* next = head->next;
			head->next = newHead;
			newHead = head;
			head = next;
		}
		return newHead;
	}
};

struct ReverseRecursion
{
	static ListNode* run(ListNode* head)
	{
		if(!head || !head->next) return head;

		ListNode* next = head->next;
		ListNode* newHead = run(next); // !!! head->next is now at end of reversed list 

		head->next->next = head;  // !!! now put head at end of reverse list
		head->next = NULL;       

		return newHead;
	}
};

template<class Func>
void test_reverse()
{
	const int count = 10;
	int a[count], b[count];
	for(int i =0; i<count; i++)
	{
		a[i] = i;
		b[i] = count - i - 1;
	}

	ListNode* head_a = buildLinkedList(a, count);
	ListNode* head_b = buildLinkedList(b, count);

	head_a = Func::run(head_a);
	assert(compareLinkedList(head_a, head_b));

	deleteLinkedList(head_a);
	deleteLinkedList(head_b);
}

// 1 <= m <= n <= length
ListNode* reverseBetween(ListNode* head, int m, int n)
{
	ListNode* curr = NULL;

	// move *curr from 1st to m-1th node
	for(int i=1; i<=m-1; i++)
	{
		if(!curr)
			curr = head;
		else
			curr = curr->next;
	}

	// record m-1th node, and move *curr to mth node
	ListNode *preNode = curr;
	curr = (curr) ? curr->next : head; // !!!!!!!!!!!!!!!!! curr could be NULL

	ListNode *revHead, *revTail;
	revHead = revTail = NULL;

	// move *curr from mth to nth node
	for(int i=m; i<=n; i++)
	{
		ListNode* next = curr->next;
		curr->next = revHead;

		if(revHead)
			revHead = curr;
		else
			revHead = revTail = curr;

		curr = next;
	}

	if(revTail)
		revTail->next = curr;

	if(preNode)
		preNode->next = revHead;

	return preNode ? head : revHead;
}

void test_reverseBetween()
{
	const int count = 5;
	int a[count] = {0,1,2,3,4};

	{
		int b[count] = {0,1,2,3,4};
		ListNode* head_a = buildLinkedList(a, count);
		ListNode* head_b = buildLinkedList(b, count);
		head_a = reverseBetween(head_a, 1, 1);
		assert(compareLinkedList(head_a, head_b));
		deleteLinkedList(head_a);
		deleteLinkedList(head_b);
	}

	{
		int b[count] = {0,1,4,3,2};
		ListNode* head_a = buildLinkedList(a, count);
		ListNode* head_b = buildLinkedList(b, count);
		head_a = reverseBetween(head_a, 3, 5);
		assert(compareLinkedList(head_a, head_b));
		deleteLinkedList(head_a);
		deleteLinkedList(head_b);
	}
}

void test_linkedList()
{
	// Reverse a Singlely Linked List
	test_reverse<ReverseIterative>();
	test_reverse<ReverseRecursion>();

	// Reverse a linked list from position m to n
	test_reverseBetween();

	// Detecting a Loop in a Singly Linked List


}