#pragma once
#include "Common.h"

template<class T>
struct List
{
private:
	NodeS<T> *head, *tail;

public:
	List() : head(0), tail(0) {}

	List(const List<T> &rhs) 
	{
		*this = rhs;
	}

	List& operator=(const List<T> &rhs)
	{
		head = tail = NULL;

		List<T>::Iterator it(rhs);
		for(; it.good(); it.advance())
			push_back(it.value());

		return *this;
	}

	void push_front(T data)
	{
		NodeS<T>* node = new NodeS<T>;
		node->data = data;
		node->next = head;

		if(!head)
		{
			tail = head = node; // init
		}
		else
		{
			head = node;	
		}
	}

	void push_back(T data)
	{
		NodeS<T>* node = new NodeS<T>;
		node->data = data;
		node->next = NULL;

		if(!tail)
		{
			tail = head = node; // init
		}
		else
		{
			tail->next = node;
			tail = node;
		}
	}

	void clear()
	{
		while(head)
		{
			NodeS<T>* node = head;
			head = head->next;
			delete node;
		}
		tail = head = NULL;
	}

	~List()
	{
		NodeS<T>* node;
		while(head)
		{
			node = head;
			head = head->next;
			delete node;
		}
	}

	struct Iterator
	{
	private:
		NodeS<T> *curr;

	public:
		Iterator(const List &l) : curr(l.head) {}

		T value() const
		{
			if(!curr) std::abort();
			return curr->data;
		}

		void advance()
		{
			if(!curr) std::abort();
			curr = curr->next;
		}

		bool good() const
		{
			return curr;
		}
	};
};