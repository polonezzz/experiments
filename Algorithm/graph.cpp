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

void Graph::addEdge(size_t from, size_t to, size_t weight)
{
	adjacent.at(from).push_front({ to, weight });
	++degree[to];
	adjacent.at(to).push_front({ from, weight });
	++degree[from];

	return;
}

 bool Graph::remove_vertex(AdjacentCont& list, size_t v)
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


void Graph::removeEdge(size_t from, size_t to)
{
	if (remove_vertex(adjacent.at(from), to))
		--degree[to];
	if (remove_vertex(adjacent.at(to), from))
		--degree[from];

	return;
}

Graph::Path Graph::DFS(size_t from, size_t to) const
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

	stack<size_t> st;
	st.push(from);

	Path path;

	while (!st.empty())
	{
		auto v = st.top();

		if (clr[v] == Color::Black)
		{
			clr[v] = Color::White;
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
			for (auto av : at(v))
			{
				if (clr[av] == Color::White)
				{
					clr[av] = Color::Grey;
					st.push(av);
				}
			}
		}
	}

	return path;
}

Graph::Path Graph::shortestPath(size_t from, size_t to) const
{
	auto vcount = count();

	if (!(from < vcount && to < vcount))
		return {};

	// make BFS

	std::queue<size_t> q;
	q.push(from);

	std::vector<bool> used(vcount);
	used[from] = true;

	std::vector<size_t> distance(vcount);
	std::vector<size_t> prev(vcount, inf);
	
	while (!q.empty())
	{
		auto v = q.front();
		q.pop();

		for (auto next : at(v))
		{
			if (!used[next])
			{
				used[next] = true;
				q.push(next);
				distance[next] = distance[v] + 1;
				prev[next] = v;
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

Graph::Path Graph::dijkstra(size_t from, size_t to) const
{
	vector<bool> marked(count(), false);
	vector<size_t> dist(count(), inf);

	vector<Edge> prev(count());
	for (size_t i = 0; i < count(); ++i)
		prev[from] = { from, i, inf };
	prev[from] = { from, from, 0 };

	queue<size_t> q;

	dist[from] = 0;
	q.push(from);

	while (!q.empty())
	{
		auto v = q.front();
		q.pop();

		for (auto& av : at(v))
		{
			if (!marked[av])
			{
				q.push(av);


				if (dist[v] + av.edgeWeigth < dist[av])
				{
					dist[av] = dist[v] + av.edgeWeigth;
					prev[av] = { v, av, av.edgeWeigth };
				}
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

Graph Graph::spanningTree() const
{
	Graph temp(count());

	std::queue<size_t> q;
	std::vector<bool> used(temp.count(), false);

	for (size_t i = 0; i < count(); ++i)
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
				if (!used[next])
				{
					used[next] = true;
					temp.addEdge(v, next.vertex, next.edgeWeigth);
					q.push(next);
				}
			}
		}
	}

	return temp;
}

Graph Graph::kruskal() const
{
	vector<Edge> edges;
	vector<size_t> colors(count());

	for (size_t i = 0; i < count(); ++i)
	{
		for (const auto& j : at(i))
			edges.emplace_back(i, j, j.edgeWeigth);

		colors[i] = i;
	}

	sort(begin(edges), end(edges));

	Graph temp(count());

	auto repaint = [&colors](auto from, auto to)
	{
		for (auto& c : colors)
		{
			if (c == from)
				c = to;
		}
	};

	for (const auto& e : edges)
	{
		if (colors[e.vBegin] != colors[e.vEnd])
		{
			temp.addEdge(e.vBegin, e.vEnd, e.weight);
			repaint(colors[e.vEnd], colors[e.vBegin]);
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

		for (size_t v = 0; v < cnt; ++v)
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
			auto edge = st.top();
			path.push_front(edge);
			st.pop();

			v = edge.vBegin;
		}
	} ;

	return path;
}

bool Graph::check_eulerian_cycle() const
{
	size_t i = 0;
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

	auto adj = at(v).front();

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
	vector<size_t> odds;
	for (size_t i = 0; i < count(); ++i)
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

void DirectedGraph::addEdge(size_t from, size_t to, size_t weight)
{
	adjacent.at(from).push_front({ to, weight });
	++degree[to];
	++outDegree[from];
		
	return;
}

void DirectedGraph::removeEdge(size_t from, size_t to)
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
	size_t i = 0;
	for (; i < count() && (getInDegree(i) == getOutDegree(i)); ++i);

	return i == count();
}

DirectedGraph::Path DirectedGraph::eulerian_trail()
{
	vector<size_t> odds;
	for (size_t i = 0; i < count(); ++i)
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
	vector<size_t> sorted;
	
	function<void(size_t)> visit = [&](size_t v)
	{
		if (clr[v] == Color::Black)
			return;

		if (clr[v] == Color::Grey)
			throw invalid_argument(nullptr);

		clr[v] = Color::Grey;
		
		for (const auto& av : at(v))
			visit(av);

		clr[v] = Color::Black;
		sorted.push_back(v);

	};

	try
	{
		for (size_t v = 0; v < count(); ++v)
			visit(v);
	}
	catch (const invalid_argument&)
	{
		return vector<size_t>();
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





