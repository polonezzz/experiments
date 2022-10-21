#pragma once

#include <forward_list>
#include <functional>
#include <memory>
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
	using Path = forward_list<Edge>;

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

	bool remove_vertex(AdjacentCont& list, size_t v);
	Path inner_eulerian_cycle(const Edge& e);
	Path inner_eulerian_trail(const Edge& e);
	
private:
	virtual bool check_eulerian_cycle() const;
	
	virtual size_t getInDegree(size_t v) const
	{
		return getDegree(v);
	};

	virtual size_t getOutDegree(size_t v) const
	{
		return getDegree(v);
	}

	virtual void addEdge(size_t from, size_t to, size_t weight = inf);

	virtual void removeEdge(size_t from, size_t to);

	virtual unique_ptr<Graph> copy_this()
	{
		return make_unique<Graph>(*this);
	}

public:
		
	Graph(size_t vertexCount)
		: adjacent(vertexCount)
		, degree(vertexCount)
		, cnt(vertexCount)
	{}

	size_t count() const { return cnt; }
		
	void add(const Edge& e)
	{
		return addEdge(e.vBegin, e.vEnd, e.weight);
	};

	void add(const initializer_list<Edge>& edges)
	{
		for (const auto& e : edges)
			add(e);
	};

	void remove(const Edge& e)
	{
		return removeEdge(e.vBegin, e.vEnd);
	}

	void remove(const initializer_list<Edge>& edges)
	{
		for (const auto& e : edges)
			remove(e);
	};

	size_t getDegree(size_t v) const
	{
		return degree[v];
	}
	
	Path DFS(size_t from, size_t to) const;
	Path shortestPath(size_t from, size_t to) const;
	
	Path dijkstra(size_t from, size_t to) const;  //Dijkstra algoithm

	Graph spanningTree() const;

	Graph kruskal() const;    // Kruskal algorithm
	
	Graph prim() const;		  // Jarnik-Prim-Dijkstra algorithm
	
	Path eulerian_cycle();

	Path eulerian_trail();
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

	size_t getDegree(size_t v) const
	{
		return getInDegree(v) + getOutDegree(v);
	}

	vector<size_t> topologicalSort() const;
	
	bool isDAG() const 
	{ 
		return 0 < topologicalSort().size(); 
	}

	Path eulerian_trail();

private:
	void addEdge(size_t from, size_t to, size_t weight = inf) override;

	void removeEdge(size_t from, size_t to) override;

	size_t getInDegree(size_t v) const override
	{
		return Graph::getDegree(v);
	};

	size_t getOutDegree(size_t v) const override
	{
		return outDegree[v];
	}

	bool check_eulerian_cycle() const override; 

	unique_ptr<Graph> copy_this() override
	{
		return make_unique<DirectedGraph>(*this);
	}
};