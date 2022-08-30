#include "graph.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <stack>

void Graph::addEdge(size_t from, size_t to)
{
	if (!isExist(from) || !isExist(to))
		return;

	auto& v = data[from];
	if (std::find(v.begin(), v.end(), to) == v.end())
	{
		v.push_back(to);
		data[to].push_back(from);
	}

	return;
}

void Graph::removeEdge(size_t from, size_t to)
{
	if (!isExist(from) || !isExist(to))
		return;

	auto& v = data[from];
	v.erase(std::find(v.begin(), v.end(), to));
	
	v = data[to];
	v.erase(std::find(v.begin(), v.end(), from));
	
	return;
}

const vector<size_t>& Graph::operator[](size_t v) const
{
	if (isExist(v)) 
		return data[v];

	throw std::out_of_range("");
}

vector<size_t> DFS(const Graph& graph, size_t from, size_t to)
{
	auto vcount = graph.size();

	if (!(from < vcount && to < vcount))
		return {};

	vector<bool> used(vcount);
	
	stack<vector<size_t>> st;
	st.push(vector<size_t>{from});

	vector<size_t> path;

	while (!st.empty())
	{
		auto vs = st.top();

		while (!vs.empty() && used[vs.back()])
			vs.pop_back();							 // find the last unvisited

		if (vs.empty())
		{
			st.pop();								 // pop empty vector
			if (!path.empty())						 // this check is needed when from and to belong to different connected components	
				path.pop_back();					 
		}
		else
		{
			auto v = vs.back();
			used[v] = true;
			
			if (v == to)
				st.swap(stack<vector<size_t>>());			
			else
				st.push(graph[v]);

			path.push_back(v);
		}
	}
	
	return path;
}

void shortestPath(const Graph& graph, size_t from, size_t to)
{
	auto vcount = graph.size();

	if (!(from < vcount && to < vcount))
		return;
	 
	// make Breadth-First Search

	std::queue<size_t> q;
	q.push(from);
	
	std::vector<bool> used(vcount);
	used[from] = true;
	
	std::vector<size_t> distance(vcount), prev(vcount);
		
	prev[from] = size_t(-1);

	while (!q.empty())					
	{
		auto v = q.front();
		q.pop();
		
		for (auto next : graph[v]) 
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

	// print path

	if (!used[to])
	{
		std::cout << "No path!";
	}
	else 
	{
		std::vector<size_t> path;
		for (size_t v = to; v != size_t(-1); v = prev[v])
			path.push_back(v);
		
		std::cout << "Path: ";
		std::copy(path.rbegin(), path.rend(), std::ostream_iterator<size_t>(std::cout," "));
			
	}
}
