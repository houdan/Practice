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
struct List
{
private:
	NodeS<T> *head, *tail;

public:
	List() : head(NULL), tail(NULL) {}

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

	bool empty() const
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

	bool empty() const
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
	List<NodeG*> neighbors; // outbound for directed graph, bi-directional for undirected graph
};

struct IntLess
{
	bool operator()(int a, int b) { return a < b; }
};

struct IntLessOrEqual
{
	bool operator()(int a, int b) { return a <= b; }
};