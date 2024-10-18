#include "stdafx.h"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <execution>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numbers>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "algo.h"
#include "bst.h"
#include "geometry.h"
#include "graph.h"
#include "list.h"
#include "misc.h"
#include "number.h"
#include "sorting.h"
#include "trie.h"

using namespace std;
/*
template <typename T, typename ...Ts>
auto concat(T t, Ts ...ts)
{
		return [=](auto ...parameters)
		{
			if (sizeof...(ts) > 1)
				return t(concat(ts...)(parameters...));
			else
				return t(parameters...);
		};
}
*/
/*
template <typename ...Ts>
int add(Ts ...ts)
{
	return int(0 + (ts + ...));	
}
*/

template <typename T>
auto filter(T predicate)
{
    return [=] (auto reduce_fn) 
	{
        return [=] (auto accumulator, auto input) 
		{
            if (predicate(input)) 
			{
                return reduce_fn(accumulator, input);
            } 
			else 
			{
                return accumulator;
            }
        };
    };
}

template <typename T>
auto fold(T fn)
{
    return [=] (auto reduce_fn) 
	{
        return [=] (auto accumulator, auto input) 
		{
            return reduce_fn(accumulator, fn(input));
        };
    };
}

class A
{
public:
	virtual int bar(int x)
	{
		return x*10;
	}
};

class B : public A
{
	int bar (int x)
	{
		return x*100;
	}
};

struct C
{
	int num;
	string name;

	C(int a, string b) : num(a), name(move(b)) {}
};

void f(int n)
{
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "thread " << n << " ended" << '\n';
}

template <typename Policy>
void executionPolicy(Policy policy)
{
	using namespace std::chrono;
	vector<uint32_t> data{ 100000000 };
	
	auto tstart = high_resolution_clock::now();
	
	for (size_t i = 0; i < 100000; ++i)
	{
		generate(policy, begin(data), end(data), [gen = mt19937{ random_device{}() }, dist = uniform_int_distribution<uint32_t>{ 0, 1000000 }]() mutable { return dist(gen); });
		sort(policy, begin(data), end(data));
	}
	
	auto tend = high_resolution_clock::now();
	cout << duration_cast<milliseconds>(tend - tstart).count() <<" - ";
}

template <typename Cont>
void fillByInts(Cont& cont, size_t count, int min, int max)
{
	mt19937 gen(random_device{}());
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

int wmain(int argc, wchar_t* argv[])
{
	
	{
		std::vector<int> data;
		fillByInts(data, 25, -10, 10);
		print(data);
		quicksort2(data.begin(), data.end());
		print(data);
		
		LinkedList<int> list{ data.begin(), data.end() };
		for (size_t i = 0; i < list.Length(); ++i)
			std::cout << list.Get(i) << " ";
		std::cout << "\n\r";

		while(!list.Empty())
			std::cout << list.Pop() << " ";
		std::cout << "\n\r";

		data = { 1,1,1,1,1,1,1,1,1,1 };
		quicksort2(data.begin(), data.end());
		print(data);

		std::vector<int> v;
		fillByInts(v, 10, 1, 10);
		print(v);
		mergeSort(v.begin(), v.end(), std::less_equal());
		print(v);

		std::vector<uint16_t> nums = { 170, 45, 75, 90, 2, 802, 2, 66 };
		auto res = radixSort(nums);

		if (binSearch(res.begin(), res.end(), 1) == res.end())
			binSearch(res.begin(), res.end(), 802);

		auto res2 = lsdRadixSort({ 170, 45, 75, 90, 2, 802, 2, 66 });
		res2 = lsdRadixSort({ 175, 45, 75, 95, 5, 805, 2, 65 });
		res2 = lsdRadixSort({ 10, 1000, 1, 10, 10000, 100, 1000, 100, 10 });

		auto strings = msdRadixSort({ "gen?", "gen ", "mt19937", "", "random_device", "dist!", " ",
									  "dist", "uint_distribution", "uint32_t", "0", "56", "42",
									  "mutable", "return", "distt", "gen" });
		
		return 0;
	}

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
		
		m.create({ 1, 3, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10});
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

		Graph g2(8);
		
		edges = { 
			{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
			{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
			{ 0, 7 }, { 1, 6 }, { 3, 4 }, { 2, 5 },
			{ 3, 6 }, { 2, 7 }, { 1, 4 }, { 0, 5 } };
		
		g2.add(edges);

		auto tree = g2.spanningTree();

		enum {A = 0, B, C, D, E, F, G, H, I, J, K};
		edges = {{ A, B, 7 }, { B, C, 8 }, { A, D, 5 }, { D, B, 9 },
				 { B, E, 7 }, { C, E, 5 }, { D, E, 15 }, { D, F, 6 },
				 { E, F, 8 }, { E, G, 9 }, { F, G, 11 },
				 { I, J, 1 }, { K, J, 2 }, { I, K, 2 } };
		
		Graph g3(K + 1);
		g3.add(edges);

		tree = g3.kruskal();
		tree = g3.prim();

		ret = g3.dijkstra(A, G);
		ret = g3.dijkstra(A, E);
		ret = g3.dijkstra(C, J);

		//Graph gNegWeight(5);
		DirectedGraph gNegWeight(5);
		gNegWeight.add({ {0,1,6}, {1,3,5}, {3,1,-2}, {0,2,7}, {1,2,8},
				 {4,0,2}, {1,4,-4}, {4,3,5 /*7*/}, {2,4,9}, {2,3,-3}});

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
			      { 1, 2 }, { 0, 4 }, { 2, 4 }, { 3, 7 }, { 8, 7 }, { 7, 4 }});
		dg = ggg.topologicalSort();
		
		ret = ggg.eulerian_trail(); 

		ggg.remove({ 0, 1 });
		ret = ggg.eulerian_trail();
		ggg.remove({ 0, 1 });

	}
	
	{
		// make push pop heap

		array<int, 10> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		array<int, 10> ra;

		copy(rbegin(a), rend(a), begin(ra));

		make_heap(begin(a), end(a));
		pop_heap(begin(a), end(a));
		*(end(a) - 1) = -1;
		push_heap(begin(a), end(a));
		
		make_heap(begin(ra), end(ra));
		sort_heap(begin(ra), end(ra));
		
		;
	}
	
	{
		auto reduce_fn = [](auto a, auto x) {return a + x; };
		auto even([](int i) { return i % 2 == 0; });
		auto twice([](int i) { return i * 2; });

		std::istream_iterator<int> it{ std::cin };
		std::istream_iterator<int> end_it;

		auto copy_and_advance([](auto it, auto input)
			{
				*it = input;
				return ++it;
			});

		std::accumulate(it, end_it,
			std::ostream_iterator<int>{std::cout, ", "},
			filter(even)(::fold(twice)(copy_and_advance)));
		std::cout << '\n';
	}
	
	auto res =  rotateQ(Quaternion(0, 1, 0, 0), Quaternion(0, 0, 1, 0), - std::numbers::pi / 4);
	
	{
		for (size_t i = 0; i < 20; ++i)
		{
			executionPolicy(execution::seq);
			executionPolicy(execution::par);
			cout << '\n';
		}
	}

	{
		auto p = std::make_shared<C>(13, "askjdlasdj");
		auto ps = std::shared_ptr<string>(p, &p->name);

		p->name = "123123";
		//auto pn = make_shared<int>(p, &p->num);
	}


	using hours = chrono::duration<double, chrono::hours::period>;
	auto hh = chrono::duration_cast<hours>(chrono::minutes(15));
	{
		std::ostringstream local;
		auto cout_buff = std::cout.rdbuf(); // save pointer to std::cout buffer
		std::cout.rdbuf(local.rdbuf()); // substitute internal std::cout buffer with
		std::cout << "some message";

		// go back to old buffer
		std::cout.rdbuf(cout_buff);
	}
	
	std::vector<std::thread> v;
    v.emplace_back(f, 1);
    v.emplace_back(f, 2);
    v.emplace_back(f, 3);
    std::list<std::thread> l;
    // copy() would not compile, because std::thread is noncopyable
 
    std::move(v.begin(), v.end(), std::back_inserter(l)); 
    for (auto& t : l) t.join();
	
	auto b = bind(&A::bar, placeholders::_2, 5);
	B xx;
	b(1, xx, 1, 2,1);
	
	Trie<string> trie;
	for (string s : {"how do you do", "how do you", "how do they", "how does he", "how do"})
	{
		istringstream iss(s);
		trie.insert(istream_iterator<string>(iss), istream_iterator<string>());
	}
	
	string sdata;
	//do
	//{
		sdata.clear();
		ostringstream os(sdata);

		getline(cin, sdata);
		//std::copy(istream_iterator<string>(cin), istream_iterator<string>(), ostream_iterator<string>(os, " "));

		istringstream is(sdata);
		if (auto sub = trie.subtrie(istream_iterator<string>(is), istream_iterator<string>()))
			sub->get().print();
	//} while (!sdata.empty());
			
	for (string s : {"how do", "how do you do", "how do", "how do they", "how do you", "how does he"})
	{
		trie.print();
		std::cout << "------------------------------------\n";

		istringstream iss(s);
		trie.remove(istream_iterator<string>(iss), istream_iterator<string>());
	}
	
	
	const size_t n = 4;
	uint8_t m[4][4]{ {1,0,0,0}, {0,0,1,0}, {1,0,1,0}, {0,1,0,0}};
	
	for (size_t k = 0; k < n; ++k)
		for (size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < n; ++j)
				m[i][j] |= m[i][k] & m[k][j];
	
	generateGrayCode();

	unordered_map<string, uint8_t> um {{"0001"s, 1}, {"0010"s, 2}, {"0011"s, 3}};
	um.emplace("0111"s, 7);

	auto rb = make_reverse_iterator(end(um));
	auto re = make_reverse_iterator(begin(um));

	for (auto it = rb; it != re; ++it)
		std::cout << (*it).first << endl;;

/*
	std::vector<Point2D> pts = {{-5,0}, {0, 5}, {0, -5}, {0, -5}, {5,0},{5,0}};
    auto isCircle = pointsMakeCircle(pts);

	std::vector<Point2D> pts2 = {{-5,0}, {0, 5}, {-2, 0}, {4, 0}, {5,0}, {7,0}};
	isCircle = pointsMakeCircle(pts2);
*/
	std::vector<Point2D> pts3 = {{1,0}, {5,0}, {7,0}};
	auto isCircle = pointsMakeCircle(pts3);

	std::vector<Point2D> pts4 = {{0,0}, {0,4}, {0,8}};
	isCircle = pointsMakeCircle(pts4);


	mt19937 gen(random_device{}());
	uniform_int_distribution<uint32_t> udis(0, 100);

	stack<uint32_t> st;
	for (size_t i = 0; i < 20; ++i)
		st.push(udis(gen));

	//sortingStack(s, greater<uint32_t>());
	sortingStack(st, less<uint32_t>());
	//sortingStack(s, greater_equal<uint32_t>());

	st.top();

	const size_t M = 6, N = 5;

	uint32_t m2[M][N];
	for (auto& row : m2)
		for (auto& item : row)
			item = udis(gen) % 5;
	printMatrix(m2);
	zeroMatrix(m2);
	printMatrix(m2);
		
	unordered_set<uint32_t> us;
	auto bcount = us.bucket_count();
	us.max_load_factor(13.0);

	us.insert(1);
	us.insert(1);

	for (size_t i = 0; i < 100; ++i)
		us.insert(udis(gen));
	
//	phi(5*7*11*11);
	
	BST<int> tree;
	tree.insert(5);
	tree.insert(2);
	tree.insert(1);
	tree.insert(4);
	tree.insert(3);
	tree.remove(2);

	tree.remove(1);
	
	tree.insert(1);
	tree.remove(1);

	tree.insert(0);
	
	std::uniform_int_distribution<> dis(-10, 10);
	for (size_t i = 0; i < 50; ++i)
		tree.insert(dis(gen));
	
	std::vector<int> sorted;
	tree.traverse([&sorted](int key){ sorted.push_back(key); });

	while (!tree.empty())
		tree.remove(dis(gen));

	tree.clear();
	
	
/*	
	auto r = sieve(121);
	r = sieve(1000000);
	
	int32_t d = 1, x = 0, y = 0;
	std::tie(d,x,y) = gcdEuclidEx2(1, 1);
	std::tie(d,x,y) = gcdEuclidEx2(10, 6);
	std::tie(d,x,y) = gcdEuclidEx2(23, 13);
	std::tie(d,x,y) = gcdEuclidEx2(46, 240);
	std::tie(d,x,y) = gcdEuclidEx2(573, 456);
	std::tie(d,x,y) = gcdEuclidEx2(31415, 14142);

	cout << "197 is " << (isPrime(197) ? "prime\n" : "not prime\n"); 
	cout << "1 000 001 is " << (isPrime(1000001) ? "prime\n" : "not prime\n"); 
	
	cout << "gcd(1, 1) = " << gcdEuclid(1, 1) <<'\n';
	cout << "gcd(10, 6) = " << gcdEuclid(10, 6) <<'\n';
	cout << "gcd(23, 13) = " << gcdEuclid(23, 13) <<'\n';
	cout << "gcd(573, 456) = " << gcdEuclid(573, 456) <<'\n';
	cout << "gcd(31415, 14142) = " << gcdEuclid(31415, 14142) <<'\n';
	
	cout << "sqrtFloor(2) = " << sqrtFloor(2) <<'\n';
	cout << "sqrtFloor(4) = " << sqrtFloor(4) <<'\n';
	cout << "sqrtFloor(45) = " << sqrtFloor(45) <<'\n';
	cout << "sqrt of 12.25 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(12.25) << '\n';
	cout << "sqrt of 47.5 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(47.5) << '\n';
*/	

	//int vol =  waterFill({2, 5, 1, 4, 6, 4, 7, 7, 6});
	
	return 0;
}

