#include "misc.h"

#include <cwctype>
#include <unordered_map>

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

