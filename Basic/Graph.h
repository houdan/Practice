#pragma once
#include "Common.h"
#include "List.h"

// Graph node
template<class T>
struct NodeG
{
	T data;
	List<NodeG*> neighbors; // outbound for directed graph, bi-directional for undirected graph
};
