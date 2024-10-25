#include "test.h"

#include <cassert>
#include <tuple>

namespace Test
{
	void bst()
	{
		BST<int> tree;
		for (auto key : { 10,2,1,8,6,3,7,4,9,15,17,12 })
			tree.insert(key);

		auto print = [](int key) { std::cout << key << " "; };

		tree.bft(print);
		std::cout << "\r\n";

		tree.dft(print);
		std::cout << "\r\n";

		tree.remove(2);
		tree.bft(print);
		
		tree.clear();
	}

	void heapsort()
	{
		std::vector<int> data = { 5, 3, 2, 10, 1, 9, 8, 6, 4, 7 };
		print(data);

		heapSort(data.begin(), data.end(), std::greater{});
		print(data);

		data = { 5, 3, 2, 10};
		print(data);

		heapSort(data.begin(), data.end(), std::greater{});
		print(data);

		data = { 5, 3};
		print(data);

		heapSort(data.begin(), data.end(), std::less{});
		print(data);
	}

	void sorting()
	{
		std::vector<int> data;
		
		fillByInts(data, 25, -10, 10);
		print(data);
		quicksort2(data.begin(), data.end());
		print(data);

		data = { 1,1,1,1,1,1,1,1,1,1 };
		quicksort2(data.begin(), data.end());
		print(data);

		std::vector<int> v;
		fillByInts(v, 10, 1, 10);
		print(v);
		mergeSort(v.begin(), v.end(), std::less_equal());
		print(v);

		auto res2 = lsdRadixSort({ 170, 45, 75, 90, 2, 802, 2, 66 });
		res2 = lsdRadixSort({ 175, 45, 75, 95, 5, 805, 2, 65 });
		res2 = lsdRadixSort({ 10, 1000, 1, 10, 10000, 100, 1000, 100, 10 });

		auto strings = msdRadixSort({ "gen?", "gen ", "mt19937", "", "random_device", "dist!", " ",
									  "dist", "uint_distribution", "uint32_t", "0", "56", "42",
									  "mutable", "return", "distt", "gen" });

	}

	void binarySearch()
	{
		std::vector<uint16_t> nums = { 170, 45, 75, 90, 2, 802, 2, 66 };
		auto res = radixSort(nums);

		assert((binSearch(res.begin(), res.end(), 1) == res.end()));
		assert((*binSearch(res.begin(), res.end(), 802) == 802));
	}

	void list()
	{
		auto less = [](auto a, auto b) { return a < b; };

		ForwardList<int16_t> l;
		l.create({ 0, 2, 4, 6, 8 });
		l.print();

		ForwardList<int16_t> m;
		m.create({ 1, 2, 3, 5, 7, 9, 9, 9 });
		m.print();
		auto l2 = l.set_union(m, less);
		l2.print();

		m.create({ 1, 3, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10 });
		auto l3 = m;
		m.print();

		l2 = l2.set_symmetric_difference(m, less);
		l2.print();
		l2.set_symmetric_difference(m, less).print();   // == l2

		l2.set_intersection(l3, less).print();
		l2.set_difference(l3, less).print();

		l.insert_after(l.before_begin(), -1);
		l.insert_after(l.before_begin(), -2);
		l.erase_after(l.insert_after(l.before_begin(), -3));
		l.print();

		l.reverse();
		l.print();

		l.insert_after(l.erase_after(l.before_begin()), 11);
		l.print();
	}

	void graph()
	{
	
		Graph g2(8);

		initializer_list<Edge> edges = {
			{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
			{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
			{ 0, 7 }, { 1, 6 }, { 3, 4 }, { 2, 5 },
			{ 3, 6 }, { 2, 7 }, { 1, 4 }, { 0, 5 } };

		g2.add(edges);

		auto tree = g2.spanningTree();

		enum { A = 0, B, C, D, E, F, G, H, I, J, K };
		edges = { { A, B, 7 }, { B, C, 8 }, { A, D, 5 }, { D, B, 9 },
				 { B, E, 7 }, { C, E, 5 }, { D, E, 15 }, { D, F, 6 },
				 { E, F, 8 }, { E, G, 9 }, { F, G, 11 },
				 { I, J, 1 }, { K, J, 2 }, { I, K, 2 } };

		Graph g3(K + 1);
		g3.add(edges);

		tree = g3.kruskal();
		tree = g3.prim();

		auto ret = g3.dijkstra(A, G);
		ret = g3.dijkstra(A, E);
		ret = g3.dijkstra(C, J);

		//Graph gNegWeight(5);
	}

	void directedGraph()
	{
		DirectedGraph g(9);

		initializer_list<Edge> edges = { { 0, 1 }, { 0, 3 }, { 1, 3 }, { 3, 4 }, { 4, 2 },
										 { 2, 5 }, { 1, 6 }, { 2, 6 }, { 7, 8 } };
		g.add(edges);

		auto ret = g.DFS(0, 2);

		ret = g.DFS(2, 3);
		g.add({ 2, 1 });
		ret = g.DFS(2, 3);
		g.remove({ 2, 1 });
		ret = g.DFS(2, 3);

		ret = g.DFS(7, 8);
		ret = g.DFS(1, 8);
		ret = g.DFS(5, 5);

		ret = g.shortestPath(0, 6);
		ret = g.shortestPath(0, 4);

		auto dg = g.topologicalSort();
		auto isDAG = g.isDAG();

		DirectedGraph gNegWeight(5);
		gNegWeight.add({ {0,1,6}, {1,3,5}, {3,1,-2}, {0,2,7}, {1,2,8},
				 {4,0,2}, {1,4,-4}, {4,3,5 /*7*/}, {2,4,9}, {2,3,-3} });

		bool nCycle = false;
		std::tie(ret, nCycle) = gNegWeight.bellman_ford(0, 1);
		std::tie(ret, nCycle) = gNegWeight.bellman_ford(3, 0);

		DirectedGraph euler(10);

		edges = {
				{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 },
				{ 4, 5 }, { 5, 6 }, { 6, 0 }, { 0, 5 }, { 2, 9 },
				{ 0, 2 }, { 2, 5 }, { 2, 7 }, { 7, 8 }, { 8, 9 }
		};

		for (const auto& e : edges)
		{
			euler.add(e);
			euler.add({ e.vEnd, e.vBegin, e.weight });
		}
		euler.add({ 3, 3 });

		ret = euler.eulerian_cycle();
		euler.remove({ 0, 5 });

		ret = euler.eulerian_trail();

		DirectedGraph ggg(9);
		ggg.add({ { 6, 8 }, { 6, 3 }, { 5, 3 }, { 5, 1 }, { 1, 0 },
				  { 1, 2 }, { 0, 4 }, { 2, 4 }, { 3, 7 }, { 8, 7 }, { 7, 4 } });
		dg = ggg.topologicalSort();

		ret = ggg.eulerian_trail();

		ggg.remove({ 0, 1 });
		ret = ggg.eulerian_trail();
		ggg.remove({ 0, 1 });

	}
}