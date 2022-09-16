#pragma once

#include <forward_list>
#include <vector>

using namespace std;

struct Edge
{
	size_t vBegin = size_t(-1);
	size_t vEnd = size_t(-1);
	size_t weight = size_t(-1);

	Edge(size_t begin, size_t end, size_t weight = size_t(-1))
		:vBegin(begin), vEnd(end), weight(weight)
	{}

	Edge() = default;

	bool operator<(const Edge& other) const
	{
		return weight < other.weight;
	}

	bool operator>(const Edge& other) const
	{
		return other.weight < weight;
	}
};


class Graph
{
	size_t cnt = 0;

public:
	static const size_t inf = size_t(-1);

protected:
	
	struct AdjacentVertex
	{
		size_t vertex;
		size_t edgeWeigth;
		
		AdjacentVertex(size_t v, size_t w = inf)
			: vertex(v), edgeWeigth(w)
		{}

		operator size_t() const
		{
			return vertex;
		}
	};

	using AdjacentCont = forward_list<AdjacentVertex>;
	
	vector<AdjacentCont> adjacent;
	vector<size_t> degree;

	const AdjacentCont& at(size_t v) const
	{
		return adjacent.at(v);
	};

public:
	using Path = forward_list<Edge>;
	
	Graph(size_t vertexCount)
		: adjacent(vertexCount)
		, degree(vertexCount)
		, cnt(vertexCount)
	{}

	size_t count() const { return cnt; }

	bool addEdge(size_t from, size_t to, size_t weight = inf);
	
	bool addEdge(const Edge& e)
	{
		return addEdge(e.vBegin, e.vEnd, e.weight);
	};

	bool removeEdge(size_t from, size_t to);
	
	bool removeEdge(const Edge& e)
	{
		return removeEdge(e.vBegin, e.vEnd);
	}

	size_t getDegree(size_t v) const
	{
		return degree[v];
	}
	
	Path DFS(size_t from, size_t to) const;
	Path shortestPath(size_t from, size_t to) const;
	
	Path dijkstra(size_t from, size_t to) const;  //Dijkstra algoithm

	Graph spanningTree();

	Graph kruskal();    // Kruskal algorithm
	
	Graph prim();		// Jarnik-Prim-Dijkstra algorithm
	
	// todo:
	// check Graph & DirectedGraph
	// Eulerian cycle  
	// Eulerian trail is a special case of Eulerian cycle

};

class DirectedGraph : public Graph
{
	//using vector<size_t> degree as inDegree;
	vector<size_t> outDegree;

public:
	DirectedGraph(size_t vertexCount)
		: Graph(vertexCount)
		, outDegree(vertexCount)
	{}

	bool addEdge(size_t from, size_t to, size_t weight = inf);
	bool removeEdge(size_t from, size_t to);
	
	size_t getDegree(size_t v) const
	{
		return getInDegree(v) + getOutDegree(v);
	}

	DirectedGraph topologicalSort() const;
	
	bool isDAG() const 
	{ 
		return 0 < topologicalSort().count(); 
	}

	//todo: 
	// find cycle in graph
	

private:
	size_t getInDegree(size_t v) const
	{
		return getDegree(v);
	};

	size_t getOutDegree(size_t v) const
	{
		return outDegree[v];
	}
};