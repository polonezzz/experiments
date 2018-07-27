
#include "number.h"

#include <tuple>

uint32_t gcdEuclid(uint32_t a, uint32_t b)
{
	while (b != 0)
	{
		auto r = a % b;
		a = b;
		b = r;
	}
	
	return a;
}

uint32_t gcdEuclid2(uint32_t a, uint32_t b)
{
	while (b != 0)
	{
		auto r = a;
		while (r >= b)
			r -= b;
		
		a = b;
		b = r;
	}
	
	return a;
}

std::tuple<int32_t, int32_t, int32_t> gcdEuclidEx(int32_t a, int32_t b)
{
	// http://e-maxx.ru/algo/export_extended_euclid_algorithm

	int32_t d = a;
	int32_t x = 1;
	int32_t y = 0;
	
	if (b == 0)
		return std::make_tuple(d, x, y);

	std::tie(d, x, y) = gcdEuclidEx(b, a % b);
	
	return std::make_tuple(d, y, x - a / b * y);
}

std::tuple<int32_t, int32_t, int32_t> gcdEuclidEx2(int32_t a, int32_t b)
{
	// 	https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm 
	
	int32_t s1 = 1, s2 = 0;
	int32_t t1 = 0, t2 = 1;
	
	if (b == 0)
		return std::make_tuple(a, s1, s2);
	else if (a == 0)
		return std::make_tuple(b, t1, t2);

	do
	{
		auto q = a / b;
		auto r = a % b;
		a = b;
		b = r;

		auto s = s1 - q*s2;
		s1 = s2;
		s2 = s;

		auto t = t1 - q*t2;
		t1 = t2;
		t2 = t;

	} while (b);

	return std::make_tuple(a, s1, t1);
}

std::tuple<bool, int32_t, int32_t> linearDiophant(int32_t a, int32_t b, int32_t c)
{
	int32_t d = 0;
	int32_t x = 0;
	int32_t y = 0;
	
	std::tie(d, x, y) = gcdEuclidEx(a, b);
	if (c % d)
		return std::make_tuple(false, 0, 0);
	else
		return std::make_tuple(true, x * c / d, y * c / d);
}

std::vector<size_t> sieve(size_t limit)  // of Eratosthenes
{
	std::vector<size_t> primes;

	if (limit < 2)
		return primes; 

	std::vector<bool> data(limit + 1);
	data[0] = data[1] = true;

	size_t n = 2;
	while (n * n <= limit)
	{
		if (!data[n])
		{
			for (size_t i = n * n; i <= limit; i += n)
				data[i] = true;
		}

		++n;
	}
	
	for (size_t i = 0; i < data.size(); ++i)
		if (!data[i])
			primes.push_back(i);		
	
	return primes;
}

size_t sqrtFloor(size_t a)
{
	if (a == 0)
		return 0;

	size_t n = 1, root = 0;

	// sum of  1 + 3 + 5 + ... equals square of member count (i.e n^2)
	
	while (n <= a)
	{
		a -= n;
		n += 2;
		++root;
	}

	return root;
}


bool isPrime(size_t number)
{
	if (number == 0 || number == 1)
		return false;

	for(auto prime : sieve(sqrtFloor(number)))
		if (number % prime == 0)
			return false;
	return true;
}

int phi(int n)  //Euler's function
{
	/*
	   ≈сли p Ч простое число, то phi(p) = p - 1 (любое число, кроме самого p, взаимно просто с ним.)
	   
	   ≈сли p Ч простое, a Ч натуральное число, то phi(p^a) = p^a - p^(a-1) 
		(числом p^a не взаимно просты только числа вида pk (k in {N}), которых p^a / p = p^{a-1} штук.)

	   ≈сли a и b взаимно простые, то phi(ab) = phi(a) * phi(b) ("мультипликативность" функции Ёйлера). 
	    (следует из китайской теоремы об остатках)

	   ќтсюда можно получить функцию Ёйлера дл€ любого n через его факторизацию:
	   если n = p1^a1 * p2^a2 * ...pk^ak , где все pi Ч простые, то
	   
	   phi(n) = phi(p1^a1) * phi(p2^a2) * ... * phi(pk^ak) 
	   = (p1^a1 - p1^(a1-1)) * (p2^a2 - p2^(a2-1)) * ... * (pk^ak - pk^(ak-1))
	   = n * (1 - 1/p1) * (1 - 1/p2) * ... * (1 - 1/pk)

	*/

	int result = n;
	for (int i = 2; i*i <= n; ++i)
	{
		if (n % i == 0) 
		{
			while (n % i == 0)
				n /= i;
			
			result -= result / i;		//n * (1 - 1/p1) * (1 - 1/p2) * ... * (1 - 1/pk)
		}
	}
	if (n > 1)
		result -= result / n;
	return result;
}