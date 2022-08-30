#pragma once

#include <initializer_list>

template <typename T>
class ForwardList;

template <typename T>
class ForwardListNode
{
	friend class ForwardList<T>;

	T data;
	ForwardListNode* next = nullptr;

public:
	T GetValue() const { return data; }
};

template <typename T>
class ForwardList
{
	using Node = typename ForwardListNode<T>;
	Node* head = nullptr;

public:
	void create(const std::initializer_list<T>& args);
	void clear();
	void reverse();
	
	template<typename Func>
	void for_each(Func func);

	Node* front() { return head; };
	
	void push_front(const T& value);
	void pop_front();
	
	//insert_after, return inserted ptr
	//erase_after, return following the erased
	//before_begin fake ptr
	// merge

	//how the sort and unique works in std::forward_list?
};

template <typename T>
void ForwardList<T>::create(const std::initializer_list<T>& args)
{
	for (auto it = rbegin(args); it != rend(args); ++it)
		push_front(*it);

	return;
}

template <typename T>
void ForwardList<T>::clear()
{
	while (head)
	{
		auto node = head;
		head = head->next;
		delete node;
	}
}

template <typename T>
void ForwardList<T>::push_front(const T& value)
{
	auto node = new ForwardListNode<T>();
	node->data = value;
	node->next = head;
	head = node;
}

template <typename T>
void ForwardList<T>::pop_front()
{
	if (head)
	{
		auto node = head;
		head = head->next;
		delete node;
	}
}

template <typename T>
void ForwardList<T>::reverse()
{
	if (!(head && head->next))
		return;

	auto node = head;
	while (head->next)
	{
		auto temp = head->next;
		head->next = head->next->next;
		temp->next = node;
		node = temp;
	}

	head = node;
	return;
}

template <typename T>
template <typename Func>
void ForwardList<T>::for_each(Func func)
{
	auto node = head;
	while (node)
	{
		func(node);
		node = node->next;
	}
}


/*
template <typename T>
ListItem<T>* insertAfter(ListItem<T>* node, const T& val)
{
	auto newNode = new ListItem<T>();
	newNode->data = val;

	if (node)
	{
		newNode->next = node->next;
		node->next = newNode;
	}
	else
	{
		node = newNode;
	}

	return newNode;
}

template <typename T>
ListItem<T>* removeAfter(ListItem<T>* node)
{
	if (node && node->next)
	{
		auto oldNode = node->next;
		node->next = oldNode->next;
		delete oldNode;
	}

	return node;
}
*/