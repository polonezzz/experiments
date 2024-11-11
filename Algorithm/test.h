#pragma once

#include "algo.h"
#include "bst.h"
#include "graph.h"
#include "heap.h"
#include "list.h"
#include "sorting.h"
#include "trie.h"
#include "trieLCRS.h"

#include <random>

namespace Test
{
	void binarySearch();
	void bst();
	void graph();
	void mazeBFS();
	void directedGraph();

	void list();
	void simpleList();
	
	void sorting();
	
	void heapsort();
	void heapSTL();

	void trie();
	void trieLCRS();

}

template <typename Cont>
void fillByInts(Cont& cont, size_t count, int min, int max)
{
	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<> dis(min, max);

	//cont.clear();

	for (size_t i = 0; i < count; ++i)
		cont.push_back(dis(gen));
}

template <typename Cont>
void print(Cont&& cont)
{
	std::copy(cont.begin(), cont.end(), std::ostream_iterator<int>{std::cout, " "});
	std::cout << "\n\r";
}
