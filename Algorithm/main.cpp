#include "stdafx.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_set>
#include <vector>

#include "algo.h"
#include "bst.h"
#include "graph.h"
#include "misc.h"
#include "number.h"
#include "sorting.h"

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	uint32_t a = 9167368, b = 3;
	uint32_t d = 0;
	int32_t x = 0;
	int32_t y = 0;
	
	std::tie(d, x, y) =	gcdEuclidEx(a, b);
	assert(a*x + b*y == d);
	
	std::tie(d, x, y) =	gcdEuclidEx(b, a);
	assert(b*x + a*y == d);
	
	std::tie(d, x, y) =	gcdEuclidEx(b, 0);
	assert(b*x == d);
	

	mt19937 gen(random_device{}());
	uniform_int<uint32_t> udis(0, 99);

	forward_list<uint32_t> list;
	for (size_t i = 0; i < 20; ++i)
		list.push_front(udis(gen) % 5);

	for (auto i : list)
		std::cout << std::setw(2) << i;
	std::cout << '\n';

	/**/

	for (auto i : list)
		std::cout << std::setw(2) << i;
	std::cout << '\n';

	const size_t M = 6, N = 5;
/*
	uint32_t m[N][N];
	for (auto& row : m)
		for (auto& item : row)
			item = udis(gen);
	printMatrix(m);
	rotateMatrix2(m, true);
	printMatrix(m);
	rotateMatrix2(m, false);
	printMatrix(m);
*/	
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
	
	//auto r = sieve(121);
	//r = sieve(1000000);
	
	cout << "197 is " << (isPrime(197) ? "prime\n" : "not prime\n"); 

	/*
	cout << "1 is " << (isPrime(1) ? "prime\n" : "not prime\n"); 
	cout << "2 is " << (isPrime(2) ? "prime\n" : "not prime\n"); 
	cout << "47 is " << (isPrime(47) ? "prime\n" : "not prime\n"); 
	cout << "121 is " << (isPrime(121) ? "prime\n" : "not prime\n"); 
	cout << "1 000 001 is " << (isPrime(1000001) ? "prime\n" : "not prime\n"); 
	*/

	
	cout << "gcd(1, 1) = " << gcdEuclid(1, 1) <<'\n';
	cout << "gcd(10, 6) = " << gcdEuclid(10, 6) <<'\n';
	cout << "gcd(23, 13) = " << gcdEuclid(23, 13) <<'\n';
	cout << "gcd(573, 456) = " << gcdEuclid(573, 456) <<'\n';
	cout << "gcd(31415, 14142) = " << gcdEuclid(31415, 14142) <<'\n';
	cout << "gcd(0, 2) = " << gcdEuclid(0, 2) <<'\n';
	cout << "gcd(0, 0) = " << gcdEuclid(0, 0) <<'\n';

	cout << "gcd(1, 1) = " << gcdEuclid2(1, 1) <<'\n';
	cout << "gcd(10, 6) = " << gcdEuclid2(6, 10) <<'\n';
	cout << "gcd(23, 13) = " << gcdEuclid2(13, 23) <<'\n';
	cout << "gcd(573, 456) = " << gcdEuclid2(456, 573) <<'\n';
	cout << "gcd(31415, 14142) = " << gcdEuclid2(14142, 31415) <<'\n';
	cout << "gcd(0, 2) = " << gcdEuclid2(0, 2) <<'\n';
	cout << "gcd(0, 0) = " << gcdEuclid2(0, 0) <<'\n';


	/*
	cout << "sqrtFloor(1) = " << sqrtFloor(1) <<'\n';
	cout << "sqrtFloor(2) = " << sqrtFloor(2) <<'\n';
	cout << "sqrtFloor(4) = " << sqrtFloor(4) <<'\n';
	cout << "sqrtFloor(6) = " << sqrtFloor(6) <<'\n';
	cout << "sqrtFloor(121) = " << sqrtFloor(121) <<'\n';
	cout << "sqrtFloor(1000) = " << sqrtFloor(1000) <<'\n';
	cout << "sqrtFloor(1002) = " << sqrtFloor(1002) <<'\n';
	cout << "sqrtFloor(45) = " << sqrtFloor(45) <<'\n';
	*/

	/*
	cout << "sqrt of 12.25 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(12.25) << '\n';
	cout << "sqrt of 47.5 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(47.5) << '\n';
	cout << "sqrt of 49 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(49) << '\n';
	cout << "sqrt of 65535 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(0xFFFF) << '\n';
	cout << "sqrt of 65536 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(0x10000) << '\n';
	cout << "sqrt of 125348 = " << setprecision(numeric_limits<double>::digits10) << sqrtHero(125348) << '\n';
	*/

	//int vol =  waterFill({2, 5, 1, 4, 6, 4, 7, 7, 6});
	

	return 0;
}

