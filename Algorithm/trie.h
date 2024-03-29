#pragma once

#include <map>
#include <type_traits>
#include <utility>

#include <boost/optional.hpp>

template<typename T>
class Trie
{
	std::map<T, Trie<T>> tries;
	bool isNode = false;

public:

	typedef boost::optional<std::reference_wrapper<Trie<T>>> Subtrie;

	template<typename It>
	void insert(It begin, It end);

	template<typename It>
	bool remove(It begin, It end);

	template<typename It>
	Subtrie subtrie(It begin, It end);

	void print(/*std::basic_ostream<>*/) const;
	
private:
	void print(/*std::basic_ostream<>*/ std::vector<T>& data) const;

};

template<typename T>
template<typename It>
void Trie<T>::insert(It begin, It end)
{
	if (begin == end)
	{
		isNode = true;
		return;
	}

	tries[*begin].insert(next(begin), end);
}

template<typename T>
template<typename It>
bool Trie<T>::remove(It begin, It end)
{
	if (begin == end)
	{
		isNode = false;
		return true;
	}

	bool ret = false;

	auto child = tries.find(*begin);
	if (child == tries.end())
		return ret;

	if (ret = child->second.remove(next(begin), end))
	{
		if (child->second.tries.empty() && !child->second.isNode)
			tries.erase(child);
	}

	return ret;
}

template<typename T>
template<typename It>
typename Trie<T>::Subtrie Trie<T>::subtrie(It begin, It end)
{
	if (begin == end)
		return ref(*this);

	auto child = tries.find(*begin);
	if (child == tries.end())
		return {};

	return child->second.subtrie(next(begin), end);
}

template<typename T>
void Trie<T>::print(/*std::basic_ostream<>*/) const
{
	std::vector<T> data;
	print(data);
}

template<typename T>
void Trie<T>::print(/*std::basic_ostream<>*/ std::vector<T>& data) const
{
	if (isNode)
	{	
		std::copy(std::begin(data), std::end(data), std::ostream_iterator<T>(std::cout, " "));
		std::cout << std::endl;
	}

	for(const auto& t : tries)
	{
		data.push_back(t.first);
		t.second.print(data);
		data.pop_back();
	}
}