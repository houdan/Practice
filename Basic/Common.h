#pragma once
#include <cstdlib>

template <class T>
struct NodeS
{
	T data;
	NodeS* next;
};

template <class T>
struct NodeD
{
	T data;
	NodeD* next;
	NodeD* prev;
};

// Binary tree node
template<class T>
struct NodeBT
{
	T data;
	NodeBT *left, *right;
};

struct IntLess
{
	bool operator()(int a, int b) { return a < b; }
};

struct IntLessOrEqual
{
	bool operator()(int a, int b) { return a <= b; }
};

struct IntGreater
{
	bool operator()(int a, int b) { return a > b; }
};

struct IntGreaterOrEqual
{
	bool operator()(int a, int b) { return a >= b; }
};