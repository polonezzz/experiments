#pragma once

#include <initializer_list>

template <typename T>
struct ListItem
{
	T data;
	ListItem* next = nullptr;
};

template <typename T>
ListItem<T>* createList(const std::initializer_list<T> args)
{
	ListItem<uint32_t>* head = nullptr;
	for (auto it = rbegin(args); it != rend(args); ++it)
	{
		auto node = new ListItem<T>();
		node->data = *it;
		node->next = head;
		head = node;
	}

	return head;
}

template <typename T>
void deleteList(ListItem<T>* head)
{
	while (head)
	{
		auto node = head;
		head = head->next;
		delete node;
	}
}

template <typename T>
ListItem<T>* reverseList(ListItem<T>* head)
{
	using Node = ListItem<T>*;

	if (!(head && head->next))
		return head;

	Node node = head;
	while (head->next)
	{
		auto temp = head->next;
		head->next = head->next->next;
		temp->next = node;
		node = temp;
	}
	
	return node;
}

template <typename T, typename Pred>
void traverseList(ListItem<T>* head, Pred pred)
{
	auto node = head;
	while (node)
	{
		pred(node);
		node = node->next;
	}
}
