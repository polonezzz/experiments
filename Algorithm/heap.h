#pragma once

template <typename It, typename Comp>
void popHeap(It begin, It end, Comp comp)
{
	if (begin == end)
		return;

	std::swap(*begin, *(end - 1));

	auto maxDist = std::distance(begin, end - 1);
	decltype(maxDist) dist = 0;

	bool canProceed = true;

	while (canProceed)
	{
		canProceed = false;

		auto parent = begin + dist;
		dist *= 2;

		if (++dist < maxDist)
		{
			auto child = begin + dist;

			if (dist + 1 < maxDist)
			{
				if (comp(*(child + 1), *child))
				{
					++dist;
					++child;
				}
			}

			if (canProceed = comp(*child, *parent))
				std::swap(*child, *parent);
		}
	}
}

template <typename It, typename Comp>
void pushHeap(It begin, It end, Comp comp)
{
	if (begin == end)
		return;

	auto child = end - 1;

	auto dist = std::distance(begin, child);

	while (dist > 0)
	{
		--dist;
		dist /= 2;

		auto parent = begin + dist;

		if (comp(*child, *parent))
		{
			std::swap(*child, *parent);
			child = parent;
		}
		else
		{
			dist = 0;			// exit do-while
		}
	}
}

template <typename It, typename Comp>
void makeHeap(It begin, It end, Comp comp)
{
	auto it = begin;

	while (it != end)
		pushHeap(begin, ++it, comp);
}


template <typename It, typename Comp>
void heapSort(It begin, It end, Comp comp)
{
	makeHeap(begin, end, std::not_fn(comp));

	auto it = end;

	while (begin != it)
	{
		popHeap(begin, it, std::not_fn(comp));
		--it;
	}
}


