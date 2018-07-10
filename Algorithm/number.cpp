
#include "number.h"

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
		(числом p^a не взаимно просты только числа вида pk (k \in \mathcal{N}), которых p^a / p = p^{a-1} штук.)

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