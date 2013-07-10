#include <iostream>

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

template<class T>
struct Stack
{
private:
	// Always insert from head
	NodeS<T> *head; 

public:
	Stack() : head(NULL) {}

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

	bool empty()
	{
		return !head;
	}
};

template<class T>
struct Queue
{
private:
	// Always insert from tail
	NodeS<T> *head, *tail;

public:
	Queue() : head(NULL), tail(NULL) {}

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

	bool empty()
	{
		return !head;
	}
};

// Binary tree node
template<class T>
struct NodeBT
{
	T data;
	NodeBT *left, *right;
};

// Graph node
template<class T>
struct NodeG
{
	T data;
	NodeS<NodeG> *children; // head of child list
};