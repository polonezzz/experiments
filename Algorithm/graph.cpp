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

bool Graph::addEdge(size_t from, size_t to, size_t weight)
{
	try
	{
		auto& adjFrom = adjacent.at(from);
		auto& adjTo = adjacent.at(to);

		if (std::find(adjFrom.begin(), adjFrom.end(), to) == adjFrom.end())
		{
			adjFrom.push_front({ to, weight });
			++degree[to];
			adjTo.push_front({ from, weight });
			++degree[from];
		}

		return true;
	}
	catch (const out_of_range&)
	{
		return false;
	}
}

bool Graph::removeEdge(size_t from, size_t to)
{
	try
	{
		auto& adjFrom = adjacent.at(from);
		auto& adjTo = adjacent.at(to);

		adjFrom.remove(to);
		--degree[to];
		adjTo.remove(from);
		--degree[from];

		return true;
	}
	catch (const out_of_range&)
	{
		return false;
	}
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

Graph Graph::spanningTree()
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

Graph Graph::kruskal()
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

Graph Graph::prim()
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
			temp.addEdge(minEdge);
		}

		minEdge = { inf, inf, inf };
	}

	return temp;
}

bool DirectedGraph::addEdge(size_t from, size_t to, size_t weight)
{
	try
	{
		auto& adj = adjacent.at(from);

		if (std::find(adj.begin(), adj.end(), to) == adj.end())
		{
			adj.push_front({ to, weight });
			++degree[to];
			++outDegree[from];
		}

		return true;
	}
	catch (const out_of_range&)
	{
		return false;
	}
}

bool DirectedGraph::removeEdge(size_t from, size_t to)
{
	try
	{
		auto& adj = adjacent.at(from);

		adj.remove(to);
		--degree[to];
		--outDegree[from];

		return true;
	}
	catch (const out_of_range&)
	{
		return false;
	}
}

DirectedGraph DirectedGraph::topologicalSort() const
{
	enum class Color
	{
		White = 0,
		Grey = 1,
		Black = 2,
	};

	vector<Color> clr(count(), Color::White);
	vector<size_t> sorted;
	sorted.reserve(count());

	function<void(size_t)> visit = [&](size_t v)
	{
		if (clr[v] == Color::Black)
			return;

		if (clr[v] == Color::Grey)
			throw invalid_argument(nullptr);

		clr[v] = Color::Grey;
		sorted.push_back(v);

		for (const auto& av : at(v))
			visit(av);

		clr[v] = Color::Black;
	};

	try
	{
		for (size_t v = 0; v < count(); ++v)
			visit(v);
	}
	catch (const invalid_argument&)
	{
		return DirectedGraph(0);
	}

	vector<size_t> mapped(sorted.size());
	for (size_t i = 0; i < sorted.size(); ++i)
		mapped[sorted[i]] = i;

	DirectedGraph sortedGraph(count());
	for (size_t i = 0; i < count(); ++i)
		for (auto j : at(i))
			sortedGraph.addEdge(mapped[i], mapped[j]);

	return sortedGraph;
}





