#include "graph.h"

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <stack>

#include <cassert>

void Graph::addEdge(Vertex from, Vertex to, Weight weight)
{
	adjacent.at(from).push_front({ to, weight });
	++degree[to];
	adjacent.at(to).push_front({ from, weight });
	++degree[from];

	return;
}

bool Graph::remove_vertex(AdjacentCont& list, Vertex v)
{
	bool res = false;

	auto prev = list.before_begin();
	auto curr = prev;
	++curr;

	while (curr != list.end())
	{
		if (res = (curr->vertex == v))
		{
			list.erase_after(prev);
			curr = list.end();
		}
		else
		{
			prev = curr++;
		}
	}

	return res;
};


void Graph::removeEdge(Vertex from, Vertex to)
{
	if (remove_vertex(adjacent.at(from), to))
		--degree[to];
	if (remove_vertex(adjacent.at(to), from))
		--degree[from];

	return;
}

Graph::Path Graph::DFS(Vertex from, Vertex to) const
{
	auto vcount = count();

	if (!(from < vcount && to < vcount))
		return {};

	enum class Color
	{
		White = 0,
		Grey = 1,
		Black = 2,
	};

	vector<Color> clr(count(), Color::White);

	stack<Vertex> st;
	st.push(from);

	Path path;

	while (!st.empty())
	{
		auto v = st.top();

		if (clr[v] == Color::Black)
		{
			st.pop();
			continue;
		}

		clr[v] = Color::Black;

		if (v == to)
		{
			auto prev = v;
			st.pop();

			while (!st.empty())
			{
				prev = st.top();
				if (clr[prev] == Color::Black)
				{
					path.push_front({ prev, v });
					v = prev;
				}
				st.pop();
			}
		}
		else
		{
			for (const auto& av : at(v))
			{
				if (clr[av.vertex] == Color::White)
				{
					clr[av.vertex] = Color::Grey;
					st.push(av.vertex);
				}
			}
		}
	}

	return path;
}

Graph::Path Graph::shortestPath(Vertex from, Vertex to) const
{
	auto vcount = count();

	if (!(from < vcount && to < vcount))
		return {};

	// make BFS

	std::queue<Vertex> q;
	q.push(from);

	std::vector<bool> used(vcount);
	used[from] = true;

	std::vector<Vertex> distance(vcount);
	std::vector<Vertex> prev(vcount, inf);

	while (!q.empty())
	{
		auto v = q.front();
		q.pop();

		for (auto& next : at(v))
		{
			if (!used[next.vertex])
			{
				used[next.vertex] = true;
				q.push(next.vertex);
				distance[next.vertex] = distance[v] + 1;
				prev[next.vertex] = v;
			}
		}
	}

	Path path;

	if (used[to])
	{
		auto v = to;

		while (prev[v] != inf)
		{
			path.push_front({ prev[v], v });
			v = prev[v];
		}
	}

	return path;
}

Graph::Path Graph::dijkstra(Vertex from, Vertex to) const
{
	vector<bool> marked(count(), false);
	vector<Weight> dist(count(), inf);

	vector<Edge> prev(count());
	for (size_t i = 0; i < count(); ++i)
		prev[from] = { from, i, inf };
	prev[from] = { from, from, 0 };

	queue<Vertex> q;

	dist[from] = 0;
	q.push(from);

	while (!q.empty())
	{
		auto v = q.front();
		q.pop();

		if (marked[v])
			continue;

		for (auto& av : at(v))
		{
			q.push(av.vertex);

			if (dist[v] + av.edgeWeigth < dist[av.vertex])
			{
				dist[av.vertex] = dist[v] + av.edgeWeigth;
				prev[av.vertex] = { v, av.vertex, av.edgeWeigth };
			}
		}

		marked[v] = true;
	}

	Path path;

	if (prev[to].weight != inf)
	{
		auto v = to;
		while (v != from)
		{
			path.push_front(prev[v]);
			v = prev[v].vBegin;
		}
	}

	return path;
}

pair<Graph::Path, bool> Graph::bellman_ford(Vertex from, Vertex to) const
{
	const auto vcnt = count();
	vector<Weight> dist(vcnt, inf);

	vector<Edge> prev(vcnt);
	for (Vertex i = 0; i < vcnt; ++i)
		prev[from] = { from, i, inf };
	prev[from] = { from, from, 0 };

	dist[from] = 0;

	for (Vertex i = 0; i < vcnt - 1; ++i)
	{
		for (Vertex v = 0; v < vcnt; ++v)
		{
			for (auto& av : at(v))
			{
				if (dist[v] != inf && dist[v] + av.edgeWeigth < dist[av.vertex])
				{
					dist[av.vertex] = dist[v] + av.edgeWeigth;
					prev[av.vertex] = { v, av.vertex, av.edgeWeigth };
				}
			}
		}
	}

	Path path;
	bool nCycle = false;

	if (prev[to].weight != inf)
	{
		vector<Vertex> marked(vcnt, false);
		
		auto v = to;
		marked[v] = true;

		Vertex x = 0;
		
		while (!(v == from || nCycle))
		{
			path.push_front(prev[v]);
			v = prev[v].vBegin;

			if (nCycle = marked[v])
				x = v;
			else
				marked[v] = true;
		}

		if (nCycle)
		{
			path.clear();
			auto v = x;
			do
			{
				path.push_front(prev[v]);
				v = prev[v].vBegin;
			} while (v != x);
		}

	}

	return make_pair(path,nCycle);
}

Graph Graph::spanningTree() const
{
	Graph temp(count());

	std::queue<Vertex> q;
	std::vector<bool> used(temp.count(), false);

	for (Vertex i = 0; i < count(); ++i)
	{
		if (used[i])
			continue;

		used[i] = true;
		q.push(i);

		while (!q.empty())
		{
			auto v = q.front();
			q.pop();

			for (auto next : at(v))
			{
				if (!used[next.vertex])
				{
					used[next.vertex] = true;
					temp.addEdge(v, next.vertex, next.edgeWeigth);
					q.push(next.vertex);
				}
			}
		}
	}

	return temp;
}

Graph Graph::kruskal() const
{
	vector<Edge> edges;
	vector<Vertex> colors(count());

	for (Vertex i = 0; i < count(); ++i)
	{
		for (const auto& j : at(i))
			edges.emplace_back(i, j.vertex, j.edgeWeigth);

		colors[i] = i;
	}

	sort(begin(edges), end(edges));

	Graph temp(count());

	for (const auto& e : edges)
	{
		if (colors[e.vBegin] != colors[e.vEnd])
		{
			temp.addEdge(e.vBegin, e.vEnd, e.weight);
			std::replace(colors.begin(), colors.end(), colors[e.vEnd], colors[e.vBegin]);
		}
	}

	return temp;
}

Graph Graph::prim() const
{
	const auto cnt = count();
	Graph temp(cnt);

	Edge minEdge = { inf, inf, inf };

	vector<bool> marked(cnt, false);
	marked[0] = true;

	bool allMarked = false;

	while (!allMarked)
	{
		allMarked = true;

		for (Vertex v = 0; v < cnt; ++v)
		{
			if (marked[v])
				continue;

			if (at(v).empty())	// isolated vertex
			{
				marked[v] = true;
				continue;
			}

			allMarked = false;

			for (const auto& adj : at(v))
			{
				if (marked[adj.vertex] && adj.edgeWeigth < minEdge.weight)
				{
					minEdge.vBegin = v;
					minEdge.vEnd = adj.vertex;
					minEdge.weight = adj.edgeWeigth;
				}
			}
		}

		if (allMarked)
			continue;

		if (minEdge.weight == inf)   // another connected component
		{
			auto it = find(begin(marked), end(marked), false);
			marked[it - begin(marked)] = true;
		}
		else
		{
			marked[minEdge.vBegin] = true;
			temp.add(minEdge);
		}

		minEdge = { inf, inf, inf };
	}

	return temp;
}

Graph::Path Graph::inner_eulerian_cycle(const Edge& e)
{
	Path path;

	stack<Edge> st;
	st.push(e);
	remove(st.top());

	size_t v = e.vEnd;

	while (!st.empty())
	{
		if (getOutDegree(v))
		{
			auto adj = at(v).front();
			st.push({ v, adj.vertex, adj.edgeWeigth });
			remove(st.top());

			v = adj.vertex;
		}
		else
		{
			auto& edge = st.top();
			path.push_front(edge);
			st.pop();

			v = edge.vBegin;
		}
	};

	return path;
}

bool Graph::check_eulerian_cycle() const
{
	Vertex i = 0;
	for (; i < count() && (getDegree(i) % 2 == 0); ++i);

	return i == count();
}

Graph::Path Graph::eulerian_cycle()
{
	if (!check_eulerian_cycle())
		return Path();

	auto temp = copy_this();

	size_t v = 0;
	while (v < temp->count() && temp->at(v).empty())   // find first non-isolated vertex
		++v;

	if (v == temp->count())
		return Path();

	const auto& adj = at(v).front();

	return temp->inner_eulerian_cycle({ v, adj.vertex, adj.edgeWeigth });
}

Graph::Path Graph::inner_eulerian_trail(const Edge& e)
{
	auto temp = copy_this();
	temp->add(e);
	auto path = temp->inner_eulerian_cycle(e);
	if (!path.empty())
		path.pop_front();

	return path;
}

Graph::Path Graph::eulerian_trail()
{
	vector<Vertex> odds;
	for (Vertex i = 0; i < count(); ++i)
		if (getDegree(i) % 2 == 1)
			odds.push_back(i);

	switch (odds.size())
	{
	case 0:
		return eulerian_cycle();

	case 2:
		return inner_eulerian_trail({ odds[0], odds[1] });

	default:
		return Path();
	}
}

void DirectedGraph::addEdge(Vertex from, Vertex to, Weight weight)
{
	adjacent.at(from).push_front({ to, weight });
	++degree[to];
	++outDegree[from];

	return;
}

void DirectedGraph::removeEdge(Vertex from, Vertex to)
{
	if (remove_vertex(adjacent.at(from), to))
	{

		--degree[to];
		--outDegree[from];
	}

	return;
}

bool DirectedGraph::check_eulerian_cycle() const
{
	Vertex i = 0;
	for (; i < count() && (getInDegree(i) == getOutDegree(i)); ++i);

	return i == count();
}

DirectedGraph::Path DirectedGraph::eulerian_trail()
{
	vector<Vertex> odds;
	for (Vertex i = 0; i < count(); ++i)
		if (getInDegree(i) != getOutDegree(i))
			odds.push_back(i);

	switch (odds.size())
	{
	case 0:
		return eulerian_cycle();

	case 2:
	{
		//begin: in = out + 1
		//end  : in = out - 1

		auto check = [this](auto lhs, auto rhs)
		{
			return getInDegree(lhs) == getOutDegree(lhs) + 1
				&& getInDegree(rhs) == getOutDegree(rhs) - 1;
		};

		Edge e;
		if (check(odds[0], odds[1]))
		{
			e = { odds[0], odds[1] };
		}
		else if (check(odds[1], odds[0]))
		{
			e = { odds[1], odds[0] };
		}

		if (e.vBegin != e.vEnd)
			return inner_eulerian_trail(e);
	}

	default:
		return Path();
	}
}

vector<size_t> DirectedGraph::topologicalSort() const
{
	enum class Color
	{
		White = 0,
		Grey = 1,
		Black = 2,
	};

	vector<Color> clr(count(), Color::White);
	vector<Vertex> sorted;

	function<void(Vertex)> visit = [&](Vertex v)
	{
		if (clr[v] == Color::Black)
			return;

		if (clr[v] == Color::Grey)
			throw invalid_argument(nullptr);

		clr[v] = Color::Grey;

		for (const auto& av : at(v))
			visit(av.vertex);

		clr[v] = Color::Black;
		sorted.push_back(v);

	};

	try
	{
		for (Vertex v = 0; v < count(); ++v)
			visit(v);
	}
	catch (const invalid_argument&)
	{
		return vector<Vertex>();
	}

	/*
	vector<size_t> mapped(sorted.size());
	size_t i = 0;
	while (!sorted.empty())
	{
		mapped[sorted.top()] = i++;
		sorted.pop();
	}

	DirectedGraph sortedGraph(count());
	for (size_t i = 0; i < count(); ++i)
		for (auto j : at(i))
			sortedGraph.addEdge(mapped[i], mapped[j]);
	*/

	reverse(begin(sorted), end(sorted));
	return sorted;
}





