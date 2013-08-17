#pragma once
#include <vector>

struct ListNode {
	int val;
	ListNode *next;
	ListNode() : next(0) {}
	ListNode(int x) : val(x), next(0) {}
};

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode() : left(0), right(0) {}
	TreeNode(int x) : val(x), left(0), right(0) {}
};

struct TreeNode2 {
	int val;
	TreeNode2 *left;
	TreeNode2 *right;
	TreeNode2 *parent;
	TreeNode2() : left(0), right(0), parent(0) {}
	TreeNode2(int x) : val(x), left(0), right(0), parent(0) {}
};

struct GraphNode {
	int val;
	std::vector<GraphNode*> neighbors; 
	GraphNode(int x) : val(x) {}
};

struct Interval {
	int start;
	int end;
	Interval() : start(0), end(0) {}
	Interval(int s, int e) : start(s), end(e) {}
};