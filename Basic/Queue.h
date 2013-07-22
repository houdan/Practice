#pragma once
#include "Common.h"

template<class T>
struct Queue
{
private:
	// Always insert from tail
	NodeS<T> *head, *tail;

public:
	Queue() : head(0), tail(0) {}

	void enqueue(T data)
	{
		NodeS<T> *node = new NodeS<T>;
		node->data = data;
		node->next = NULL;

		if(!head)
		{
			head = node;
			tail = head;
		}
		else
		{
			tail->next = node;
			tail = node;
		}
	}

	T dequeue()
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
