#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <numeric>
#include <stack>
#include <string>

template <typename It>
void quicksort(It begin, It end)
{
	if (std::distance(begin, end) < 2)
		return;

	auto pivot = *begin;
	auto mid = std::partition(begin, end, [pivot](auto val) { return val < pivot; });   // less | greater or equal
	auto mid2 = std::partition(mid, end, [pivot](auto val) { return !(pivot < val); }); // equal | greater

	auto f = std::async(quicksort<It>, mid2, end);	// greater than pivot
	quicksort(begin, mid);							// less than pivot
	
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

template <typename It, typename Pred>
void insertSort(It begin, It end, Pred pred)		//random access iterators
{
	for (auto i = begin; i != end; ++i)
		std::rotate(std::upper_bound(begin, i, *i, pred), i, i + 1);
}


template <typename It, typename Pred>
void mergeInplace(It lbegin, It lend, It rbegin, It rend, Pred pred)
{
	while (lbegin != lend && rbegin != rend)
	{
		if (!pred(*lbegin, *rbegin))
		{
			std::rotate(lbegin, rbegin, rbegin + 1);

			++lend;
			++rbegin;
		}
		++lbegin;
	}
}

template <typename It, typename Pred>
void mergeSort(It begin, It end, Pred pred)			//random access iterators
{
	auto size = std::distance(begin, end);
	decltype(size) step = 1;

	while (step < size)
	{
		auto i = begin;
		while (i != end)
		{
			auto j = i + std::min(step, std::distance(i, end));
			auto k = j + std::min(step, std::distance(j, end));
			mergeInplace(i, j, j, k, pred);		
			i = k;
		}
		step *= 2;
	}
}


template<typename T, typename Pred>
void sortingStack(std::stack<T>& s, Pred pred)
{
	if (s.size() < 2)
		return;

	std::stack<T> temp;
	
	while (!s.empty())
	{
		auto i = s.top();
		s.pop();

		while (!temp.empty())
		{
			auto j = temp.top();
			if (!pred(j, i))
				break;
			
			temp.pop();
			s.push(j);
		}

		temp.push(i);
	}

	s.swap(temp);
}

std::vector<uint64_t> lsdRadixSort(std::vector<uint64_t> data);

std::vector<std::string> msdRadixSort(const std::vector<std::string>& data);
std::vector<size_t> inner_msdRadixSort(const std::vector<std::string>& data, const std::vector<size_t>& pos, size_t index);


