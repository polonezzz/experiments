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

#include "geometry.h"
#include "misc.h"
#include "number.h"
#include "trie.h"

#include "test.h"

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


int wmain(int argc, wchar_t* argv[])
{
	Test::heapsort();

	return 0;
	
	
	
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

