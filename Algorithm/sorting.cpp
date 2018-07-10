
#include "sorting.h"

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