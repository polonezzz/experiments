#pragma once

#include <algorithm>
#include <future>
#include <string>

template <typename It>
void quicksort(It begin, It end)
{
	if (std::distance(begin, end) < 2)
		return;

	auto pivot = *begin;
	auto mid = std::partition(begin, end, [pivot](auto val) { return val < pivot; });
	auto mid2 = std::partition(mid, end, [pivot](auto val) { return !(pivot < val); });

	auto f = std::async(quicksort<It>, mid2, end);	// high part
	quicksort(begin, mid);							// low part		
	
	return f.get();
}


