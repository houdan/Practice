#pragma once

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
