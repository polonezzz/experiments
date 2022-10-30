#include <cwctype>
#include <unordered_map>

#include "misc.h"

#include <array>
#include <bitset>
#include <stack>

bool _isCharsUnique(std::wstring& str)
{
	if (str.empty() || str.length() == 1)
		return true;

	auto half = str.begin() + str.length() / 2;
	std::wstring lhs(str.begin(), half);
	std::wstring rhs(half, str.end());

	if (_isCharsUnique(lhs) && _isCharsUnique(rhs))		// using modified mergesort
	{
		str.clear();
		
		auto lit = lhs.begin(); 
		auto rit = rhs.begin();

		for (; lit != lhs.end() && rit != rhs.end(); )
		{
			if (*lit == *rit)
				return false;
			
			str.push_back((*lit < *rit) ? *lit++ : *rit++);
		}

		str.append(lit, lhs.end());
		str.append(rit, rhs.end());

		return true;
	}

	return false;
}

bool isCharsUnique(std::wstring str)
{
	return _isCharsUnique(str);
}

/*  
	assert(isCharsUnique(L"1"));
	assert(!isCharsUnique(L"11"));
	assert(isCharsUnique(L"91"));
	assert(isCharsUnique(L"qwerty"));
	assert(!isCharsUnique(L"abbdee"));

*/

bool isPalindrome(const std::wstring& str)
{
	using namespace std;
	unordered_map<wstring::value_type, size_t> map;

	for (auto ch : str)
		if (iswalpha(ch))
			++map[tolower(ch)];

	size_t oddCount = 0;
	for (auto i : map)
		oddCount += (i.second & 1);

	return oddCount < 2;
}

/*
	assert(isPalindrome(L"Tact , Coa ,,"));
	assert(!isPalindrome(L"common"));
	assert(isPalindrome(L"ccommon"));
	assert(!isPalindrome(L"aaarrggh!"));
*/

void towersOfHanoi(size_t from, size_t to, size_t mid, size_t count)
{
	if (count < 2)
	{
		printf("%zu : %zu -> %zu\n", count, from, to);
		return;
	}

	towersOfHanoi(from, mid, to, count - 1);
	printf("%zu : %zu -> %zu\n", count, from, to);
	towersOfHanoi(mid, to, from, count - 1);
	
	return;
}

void towersOfHanoi(size_t count)
{
	size_t from = 1, mid = 2, to = 3;
	towersOfHanoi(from, mid, to, count);

	return;
}

void grayCode(int n)
{
	for (int k = 1 << n; --k >= 0;)
	{
		int s = k ^ (k >> 1);
		
		for (int i = 0; i < n; i++) 
			printf("%d", (s >> i) & 1);
		printf("\n");
	}
}


void generateGrayCode(/*const size_t size*/)
{
	const size_t size = 4;
	std::bitset<size> bits;

	auto Gray = [&bits, size](auto gray, auto rgray, size_t k) -> void
	{
		if (k == size + 1)
		{
			std::cout << bits << std::endl;
		}
		else
		{
			bits.reset(k);
			gray(gray, rgray, k + 1);
			bits.set(k);
			rgray(gray, rgray, k + 1);
		}
	};

	auto RGray = [&bits, size](auto gray, auto rgray, size_t k) -> void
	{
		if (k == size + 1)
		{
			std::cout << bits << std::endl;
		}
		else
		{
			bits.set(k);
			gray(gray, rgray, k + 1);
			bits.reset(k);
			rgray(gray, rgray, k + 1);
		}
	};

	Gray(Gray, RGray, 1);
}
