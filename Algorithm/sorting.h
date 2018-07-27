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

template <typename It, typename Pred>
void selectSort(It begin, It end, Pred pred)         //random access iterators
{
	if (begin == end)
		return;

	for (auto i = begin; i != end - 1; ++i)
	{
		auto m = i;
		for (auto j = i + 1; j != end; ++j)
		{
			if (pred(*j, *m))
				m = j;
		}
		
		if (i != m)
			std::swap(*i, *m);
	}
}

template <typename It, typename Pred>
void bubbleSort(It begin, It end, Pred pred)		//random access iterators
{
	while (begin != end)
	{
		--end;
		for (auto j = begin; j != end; ++j)
			if (!pred(*j, *(j + 1)))
				swap(*j, *(j + 1));
	} 
}

