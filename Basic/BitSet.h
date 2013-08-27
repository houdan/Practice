#pragma once
#include <cstdlib>
#include <string>

struct BitSet
{
	BitSet(int size_)
	{
		size = size_ >> 3;  // size / 8
		buffer = new char[size]; 
		memset(buffer, 0, size);
	}

	void Set(int pos)
	{
		int index = pos >> 3;
		if(index >= size) std::abort();
		int offset = pos & 7; // pos % 8 == pos & (8-1)
		buffer[index] |= 1 << offset;
	}

	bool Get(int pos)
	{
		int index = pos >> 3;
		if(index >= size) std::abort();
		int offset = pos & 7; // pos % 8 == pos & (8-1)
		return (buffer[index] & 1 << offset) != 0;
	}

	int Size() { return size; }

	~BitSet()
	{
		delete buffer;
	}

private:
	char *buffer;
	int size;
};