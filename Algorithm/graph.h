#pragma once

#include <vector>

class Graph
{
	std::vector<std::vector<size_t>> data;

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

	const std::vector<size_t>& operator[](size_t v) const;
};

void shortestPath(const Graph& graph, size_t from, size_t to);

