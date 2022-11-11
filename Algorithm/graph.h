#pragma once

#include <forward_list>
#include <functional>
#include <memory>
#include <vector>

using namespace std;

using Vertex = size_t;
using Weight = int64_t;

struct Edge
{
	Vertex vBegin = std::numeric_limits<Vertex>::max();
	Vertex vEnd = std::numeric_limits<Vertex>::max();
	
	static const Weight inf = std::numeric_limits<Weight>::max();
	Weight weight = inf;

	Edge(Vertex begin, Vertex end, Weight weight = inf)
		:vBegin(begin), vEnd(end), weight(weight)
	{}

	Edge() = default;

	bool operator<(const Edge& other) const
	{
		return weight < other.weight;
	}

	bool operator>(const Edge& other) const
	{
		return other < *this;
	}
};

class Graph
{
	size_t cnt = 0;

public:
	
	static const Weight inf = std::numeric_limits<Weight>::max();
	using Path = forward_list<Edge>;

protected:
	
	struct AdjacentVertex
	{
		Vertex vertex;
		Weight edgeWeigth;
		
		AdjacentVertex(Vertex v, Weight w = inf)
			: vertex(v), edgeWeigth(w)
		{}
	};

	using AdjacentCont = forward_list<AdjacentVertex>;
	
	vector<AdjacentCont> adjacent;
	vector<size_t> degree;

	const AdjacentCont& at(Vertex v) const
	{
		return adjacent.at(v);
	};

	bool remove_vertex(AdjacentCont& list, Vertex v);
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

	virtual void addEdge(Vertex from, Vertex to, Weight weight = inf);

	virtual void removeEdge(Vertex from, Vertex to);

	virtual unique_ptr<Graph> copy_this()
	{
		return make_unique<Graph>(*this);
	}

public:
		
	Graph(Vertex vertexCount)
		: adjacent(vertexCount)
		, degree(vertexCount)
		, cnt(vertexCount)
	{}

	Vertex count() const { return cnt; }
		
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

	size_t getDegree(Vertex v) const
	{
		return degree[v];
	}
	
	Path DFS(Vertex from, Vertex to) const;
	Path shortestPath(Vertex from, Vertex to) const;
	
	Path dijkstra(Vertex from, Vertex to) const;  //Dijkstra algoithm

	pair<Path,bool> bellman_ford(Vertex from, Vertex to) const; //Bellman - Ford algorithm

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
	DirectedGraph(Vertex vertexCount)
		: Graph(vertexCount)
		, outDegree(vertexCount)
	{}

	size_t getDegree(Vertex v) const
	{
		return getInDegree(v) + getOutDegree(v);
	}

	vector<Vertex> topologicalSort() const;
	
	bool isDAG() const 
	{ 
		return 0 < topologicalSort().size(); 
	}

	Path eulerian_trail();

private:
	void addEdge(Vertex from, Vertex to, Weight weight = inf) override;

	void removeEdge(Vertex from, Vertex to) override;

	size_t getInDegree(Vertex v) const override
	{
		return Graph::getDegree(v);
	};

	size_t getOutDegree(Vertex v) const override
	{
		return outDegree[v];
	}

	bool check_eulerian_cycle() const override; 

	unique_ptr<Graph> copy_this() override
	{
		return make_unique<DirectedGraph>(*this);
	}
};