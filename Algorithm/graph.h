#pragma once

#include <vector>

using namespace std;

class Graph
{
	vector<vector<size_t>> data;

	bool isExist(size_t vertex) const
	{
		return vertex < size();
	}

public:
	Graph(size_t vertexCount)
		: data(vertexCount)
	{}

	size_t size() const { return data.size(); }

	void addEdge(size_t from, size_t to);
	void removeEdge(size_t from, size_t to);

	const vector<size_t>& operator[](size_t v) const;
};

vector<size_t> DFS(const Graph& graph, size_t from, size_t to);

void shortestPath(const Graph& graph, size_t from, size_t to);

