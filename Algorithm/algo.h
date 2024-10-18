#pragma once

#include <array>
#include <iterator>

int waterFill(const std::array<int, 9>& land);

double sqrtHero(double a, double precision = 10e-10);

template<std::random_access_iterator It, typename Value>
It binSearch(It begin, It end, const Value& val)
{
	auto res = end;

	while (begin != end)
	{
		auto mid = begin + (end - begin) / 2;
		
		if (*mid < val)
		{
			begin = mid + 1;
		}
		else if (*mid > val)
		{
			end = mid;
		}
		else
		{
			res = mid;
			begin = end;
		}
	}

	return res;
}

