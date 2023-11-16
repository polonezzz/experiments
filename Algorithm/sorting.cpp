
#include "sorting.h"

#include <iterator>

using namespace std;

vector<uint64_t> lsdRadixSort(vector<uint64_t> data)
{
	const size_t base = 10;

	auto maxVal = *max_element(data.begin(), data.end());
	size_t exp = 1;

	array<size_t, base> count = { 0, };
	std::vector<uint64_t> temp(data.size(), 0);

	while (maxVal / exp)
	{
		for (auto v : data)
			++count[v / exp % base];

		size_t sum = 0;
		for (auto& c : count)
		{
			sum += c;
			c = sum - c;
		}

		for (size_t i = 0; i != data.size(); ++i)
		{
			auto v = data[i] / exp % base;
			temp[count[v]] = data[i];
			++count[v];
		}

		copy(temp.begin(), temp.end(), data.begin());

		exp *= base;
		count.fill(0);
	}

	return data;
}

vector<string> msdRadixSort(const vector<string>& data)
{
	vector<size_t> pos(data.size(), 0);

	for (size_t i = 0; i != pos.size(); ++i)
		pos[i] = i;

	auto pp = inner_msdRadixSort(data, pos, 0);
	
	vector<string> sorted;
	sorted.reserve(data.size());

	for (auto p : pp)
		sorted.push_back(data[p]);

	return sorted;
}

// TO-D0: use stack instead of recursive calls

vector<size_t> inner_msdRadixSort(const vector<string>& data, const vector<size_t>& pos, size_t index)
{
	if (pos.size() < 2)
		return pos;

	size_t maxLen = 0;
	for (auto p : pos)
		if (maxLen < data[p].length())
			maxLen = data[p].length();

	if (!(index < maxLen))
		return pos;

	using Char = typename string::value_type;
	array<size_t, numeric_limits<Char>::max() + 1> count = { 0, };
	count[count.size() - 1] = pos.size();

	std::vector<size_t> temp(pos.size(), 0);

	for (auto p : pos)
		++count[index < data[p].length() ? data[p][index] : 0];

	size_t sum = 0;
	for (auto& c : count)
	{
		sum += c;
		c = sum - c;
	}

	for (size_t i = 0; i != pos.size(); ++i)
	{
		const auto& s = data[pos[i]];
		auto c = index < s.length() ? s[index] : 0;

		temp[count[c]] = pos[i];
		++count[c];
	}

	vector<size_t> resPos;
	size_t diff = 0;
	for (auto c : count)
	{	
		if (diff == c)
			continue;

		auto subPos = inner_msdRadixSort(data, {temp.begin() + diff, temp.begin() + c}, index + 1);
		move(subPos.begin(), subPos.end(), back_inserter(resPos));
		diff = c;
	}
	
	return resPos;
}
