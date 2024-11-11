#pragma once


// https://en.wikipedia.org/wiki/Trie#Implementation_strategies
// https://en.wikipedia.org/wiki/Left-child_right-sibling_binary_tree

#include <iterator>
#include <vector>

#include "list.h"

using std::vector;

template <typename T>
class LCRSTrie
{
	
	struct Node
	{
		Node* child = nullptr;
		Node* sibling = nullptr;
		T data;
	};


	Node* root;

public:
	LCRSTrie()
		: root(new Node)
	{

	}

	LCRSTrie(const LCRSTrie& src) = delete;
	LCRSTrie(LCRSTrie&& src) = delete;
	LCRSTrie& operator=(const LCRSTrie& src) = delete;
	LCRSTrie& operator=(LCRSTrie&& src) = delete;

	~LCRSTrie()
	{
		clear(root);
	}

	template <typename It>
	void insert(It begin, It end)
	{
		return insert_(root, begin, end);
	}

	template <typename It>
	void remove(It begin, It end)
	{
		remove_(root, begin, end);

		return;
	}

	template <typename It>
	vector<SimpleList<T>> complete(It prefixBegin, It prefixEnd)
	{
		return complete_(root, prefixBegin, prefixEnd);
	}
	

private:
	template <typename It>
	vector<SimpleList<T>> complete_(Node* node, It begin, It end)
	{
		vector<SimpleList<T>> res;

		auto child = node->child;

		if (begin != end)
		{
			while (child)
			{
				if (child->data == *begin)
					return complete_(child, std::next(begin), end);

				child = child->sibling;
			}
		}
		else
		{
			while (child)
			{
				auto data = complete_(child, begin, end);
				
				if (data.empty())
				{
					res.emplace_back(SimpleList{ child->data });
				}
				else
				{
					for (auto& slist : data)
					{
						slist.Push(child->data);
						res.emplace_back(std::move(slist));

					}
				}
				child = child->sibling;
			}
		}
		return res;
	}

	
	template <typename It>
	void insert_(Node* node, It begin, It end)
	{
		if (begin == end)
			return;

		if (auto child = node->child)
		{
			while (true)
			{
				if (child->data == *begin)
				{
					break;
				}
				else if (child->sibling)
				{
					child = child->sibling;
				}
				else
				{
					child->sibling = new Node;
					child = child->sibling;
					child->data = *begin;
					break;
				}
			}

			insert_(child, std::next(begin), end);
		}
		else
		{
			node->child = new Node;
			node->child->data = *begin;
			insert_(node->child, std::next(begin), end);
		}
	}

	template <typename It>
	bool remove_(Node* node, It begin, It end)
	{

		if (begin == end)
			return true;

		bool ret = false;

		if (auto child = node->child)
		{
			if (child->data == *begin)
			{
				ret = remove_(child, std::next(begin), end);
				if (ret && child->child == nullptr)
				{
					node->child = child->sibling;
					delete child;
					child = nullptr;
				}
			}
			else
			{
				Node* prev = nullptr;

				while (child->sibling)
				{
					prev = child;
					child = child->sibling;

					if (child->data == *begin)
					{
						ret = remove_(child, std::next(begin), end);

						if (ret && child->child == nullptr)
						{
							prev->sibling = child->sibling;
							delete child;
							child = nullptr;
						}

						break;
					}
				}
			}
		}

		return ret;
	}


	void clear(Node* node)
	{
		if (node == nullptr)
			return;

		clear(node->child);
		clear(node->sibling);

		delete node;
		node = nullptr;
	}

};