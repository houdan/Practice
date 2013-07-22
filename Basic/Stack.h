#pragma once
#include "Common.h"

template<class T>
struct Stack
{
private:
	// Always insert from head
	NodeS<T> *head; 

public:
	Stack() : head(0) {}

	void push(T data)
	{
		NodeS<T> *node = new NodeS<T>;
		node->data = data;
		node->next = head;
		head = node;
	}

	T pop()
	{
		if(!head)
			std::abort();

		NodeS<T> *node = head;
		head = head->next;
		T data = node->data;
		delete node;
		return data;
	}

	bool empty() const
	{
		return !head;
	}
};