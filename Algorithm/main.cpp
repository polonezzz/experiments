#include "stdafx.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <queue>
#include <random>
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
auto map(T fn)
{
    return [=] (auto reduce_fn) 
	{
        return [=] (auto accumulator, auto input) 
		{
            return reduce_fn(accumulator, fn(input));
        };
    };
}


int wmain(int argc, wchar_t* argv[])
{
	Trie<string> trie;
	for (string s : {"how do you do", "how do you", "how do they", "how does he", "how do"})
	{
		istringstream iss(s);
		trie.insert(istream_iterator<string>(iss), istream_iterator<string>());
	}
	
	for (string s : {"how do", "how do you do", "how do", "how do they", "how do you", "how does he"})
	{
		trie.print();
		std::cout << "------------------------------------\n";

		istringstream iss(s);
		trie.remove(istream_iterator<string>(iss), istream_iterator<string>());
	}

	std::istream_iterator<int> it {std::cin};
    std::istream_iterator<int> end_it;

    auto even  ([](int i) { return i % 2 == 0; });
    auto twice ([](int i) { return i * 2; });

    auto copy_and_advance ([](auto it, auto input) 
	{
        *it = input;
        return ++it;
    });

    std::accumulate(it, end_it,
            std::ostream_iterator<int>{std::cout, ", "},
            filter(even)(::map(twice)(copy_and_advance)));
    std::cout << '\n';
	
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
	uniform_int<uint32_t> udis(0, 100);

	stack<uint32_t> s;
	for (size_t i = 0; i < 20; ++i)
		s.push(udis(gen));

	//sortingStack(s, greater<uint32_t>());
	sortingStack(s, less<uint32_t>());
	//sortingStack(s, greater_equal<uint32_t>());

	s.top();

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

/*
	Graph g(6);
	g.addEdge(0, 1);
	g.addEdge(0, 3);
	g.addEdge(1, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 2);
	g.addEdge(2, 5);
	shortestPath(g, 0, 5);
*/	
	
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

	std::vector<int32_t> data;
	data.reserve(20);
	for (size_t i = 0; i < data.capacity(); ++i)
		data.push_back(dis(gen));

	quicksort(data.begin(), data.end());
	
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

