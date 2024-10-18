#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <numeric>
#include <stack>
#include <string>
#include <type_traits>

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

template <typename It>
void quicksort2(It begin, It end)
{
	if (std::distance(begin, end) < 2)
		return;

	auto pivot = *begin;
	
	auto lt = begin;
	auto eq = begin;
	auto gt = end;

	--gt;

	while (eq <= gt)
	{
		if (*eq < pivot)
			std::swap(*lt++, *eq++);
		else if (pivot < *eq)
			std::swap(*eq, *gt--);
		else
			++eq;
	};

	quicksort2(begin, lt);
	quicksort2(eq, end);

	return ;
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
void mergeInplace(It begin, It mid, It end, Pred pred)
{
	while (begin != mid && mid != end)
	{
		if (!pred(*begin, *mid))
		{
			std::rotate(begin, mid, mid + 1);

			++mid;
		}
		++begin;
	}
}

template <typename It, typename Pred>
void mergeSort(It begin, It end, Pred pred)			//random access iterators
{
	//complexity is O(logn * n^2)
	// if linked list is being sorted then complexity is  O(n*logn)

	auto size = std::distance(begin, end);
	decltype(size) step = 1;

	while (step < size)
	{
		auto i = begin;
		while (i != end)
		{
			auto j = i + std::min(step, std::distance(i, end));
			auto k = j + std::min(step, std::distance(j, end));
			mergeInplace(i, j, k, pred);		
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

template <typename T, size_t Base = 10>
std::vector<T> radixSort(std::vector<T> data, std::enable_if_t<std::is_unsigned<T>::value, bool> = true)
{
	T largest = *std::max_element(data.begin(), data.end());
	T divisor = 1;

	std::vector<std::vector<T>> buckets{Base};

	while (largest / divisor)
	{
		for (auto num : data)
		{
			auto digit = num / divisor % Base;
			buckets[digit].push_back(num);
		}

		auto it = data.begin();
		for (auto& b : buckets)
		{
			it = std::move(b.begin(), b.end(), it);
			b.clear();
		}

		divisor *= Base;
	}

	return data;
}

std::vector<uint64_t> lsdRadixSort(std::vector<uint64_t> data);
std::vector<std::string> msdRadixSort(const std::vector<std::string>& data);
std::vector<size_t> inner_msdRadixSort(const std::vector<std::string>& data, const std::vector<size_t>& pos, size_t index);


