#pragma once

#include <initializer_list>
#include <exception>

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
public:
	using Node = typename ForwardListNode<T>;
	
	ForwardList() = default;

	ForwardList(const ForwardList& other)
	{
		auto id = [](auto a) { return a; };

		auto temp = other.map(id);
		std::swap(fakeHead, temp.fakeHead);
	}
	
	ForwardList& operator=(const ForwardList& other)
	{
		auto temp = other;
		std::swap(fakeHead, temp.fakeHead);
		return *this;
	}

	ForwardList(ForwardList&& other) noexcept
	{
		fakeHead = other.fakeHead;
		other.head = nullptr;
	}
	
	ForwardList& operator=(ForwardList&& other) noexcept
	{
		auto temp = std::move(other);
		std::swap(fakeHead, temp.fakeHead);
		return *this;
	}

	~ForwardList() 
	{ 
		clear(); 
	}

	void create(const std::initializer_list<T>& args)
	{
		clear();

		for (auto it = rbegin(args); it != rend(args); ++it)
			push_front(*it);

		return;
	}

	void clear()
	{
		while (head)
		{
			auto node = head;
			head = head->next;
			delete node;
		}
	}
	
	void reverse()
	{
		if (!(head && head->next))
			return;

		Node* prev = nullptr;
		while (head)
		{
			auto temp = head->next;
			head->next = prev;
			prev = head;
			head = temp;
		}

		head = prev;

		return;
	}
	
	template<typename Functor>
	ForwardList<T> map(Functor f) const
	{
		ForwardList<T> newList;
		auto newNode = newList.before_begin();

		auto node = head;
		while (node)
		{
			newNode = newList.insert_after(newNode, f(node->data));
			node = node->next;
		}

		return newList;
	}

	Node* before_begin() { return &fakeHead; }
	Node* front() { return head; };
	
	void push_front(const T& value)
	{
		auto node = new ForwardListNode<T>();
		node->data = value;
		node->next = head;
		head = node;
	}

	void pop_front()
	{
		if (head)
		{
			auto node = head;
			head = head->next;
			delete node;
		}
	}

	Node* insert_after(Node* node, const T& val)
	{
		auto newNode = new Node;
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

	Node* erase_after(Node* node)
	{
		Node* res = nullptr;

		if (node && node->next)
		{
			auto oldNode = node->next;
			res = oldNode->next;
			node->next = res;
			delete oldNode;
		}

		return res;
	}
		
	template <typename Compare>
	void merge(ForwardList<T>& other, Compare comp)
	{
		ForwardList<T> temp;
		auto node = temp.before_begin();

		while (head && other.head)
		{
			if (comp(other.head->data, head->data))
			{
				node->next = other.head;
				other.head = other.head->next;
			}
			else
			{
				node->next = head;
				head = head->next;
			}

			node = node->next;
		}

		if (other.head)
		{
			node->next = other.head;
			other.head = nullptr;
		}
		else
		{
			node->next = head;
		}

		fakeHead = temp.fakeHead;
		temp.head = nullptr;
	}

	template<typename Compare>
	ForwardList<T> set_union(const ForwardList<T>& other, Compare comp) const    // elements of this + ones of other not in this
	{
		ForwardList<T> res;
		auto node = res.before_begin();

		auto lhs = head;
		auto rhs = other.head;

		while (lhs && rhs)
		{
			if (comp(rhs->data, lhs->data))
			{
				node = res.insert_after(node, rhs->data);
				rhs = rhs->next;
			}
			else if (!comp(lhs->data, rhs->data))
			{
				rhs = rhs->next;
			}
			else
			{
				node = res.insert_after(node, lhs->data);
				lhs = lhs->next;
			}
		}

		res.copy_tail_after(res.copy_tail_after(node, lhs), rhs);
		return res;
	}

	template <typename Compare>
	ForwardList<T> set_intersection(const ForwardList<T>& other, Compare comp) const
	{
		ForwardList<T> res;
		auto node = res.before_begin();

		auto lhs = head;
		auto rhs = other.head;

		while (lhs && rhs)
		{
			if (comp(rhs->data, lhs->data))
			{
				rhs = rhs->next;
			}
			else if (comp(lhs->data, rhs->data))
			{
				lhs = lhs->next;
			}
			else
			{
				node = res.insert_after(node, lhs->data);
				lhs = lhs->next;
				rhs = rhs->next;
			}
		}

		return res;
	}

	template <typename Compare>
	ForwardList<T> set_difference(const ForwardList<T>& other, Compare comp) const
	{
		ForwardList<T> res;
		auto node = res.before_begin();

		auto lhs = head;
		auto rhs = other.head;

		while (lhs && rhs)
		{
			if (comp(rhs->data, lhs->data))
			{
				rhs = rhs->next;
			}
			else if (!comp(lhs->data, rhs->data))
			{
				lhs = lhs->next;
				rhs = rhs->next;
			}
			else
			{
				node = res.insert_after(node, lhs->data);
				lhs = lhs->next;
			}
		}

		res.copy_tail_after(node, lhs);
		return res;
	}

	template <typename Compare>
	ForwardList<T> set_symmetric_difference(const ForwardList<T>& other, Compare comp) const
	{
		ForwardList<T> res;
		auto node = res.before_begin();

		auto lhs = head;
		auto rhs = other.head;

		while (lhs && rhs)
		{
			if (comp(rhs->data, lhs->data))
			{
				node = res.insert_after(node, rhs->data);
				rhs = rhs->next;
			}
			else if (!comp(lhs->data, rhs->data))
			{
				lhs = lhs->next;
				rhs = rhs->next;
			}
			else
			{
				node = res.insert_after(node, lhs->data);
				lhs = lhs->next;
			}
		}

		res.copy_tail_after(res.copy_tail_after(node, lhs), rhs);
		return res;
	}
	
	/*
	template<typename Compare>
	void includes(ForwardList<T>& other, Compare comp);  //true if other is a subsequence of this, otherwise false.

	how the sort and unique works in std::forward_list?
	*/

	void print(const std::wstring& delim = L" ")
	{
		if (!head)
		{
			std::wcout << L"<empty>\n";
			return;
		}

		auto node = head;
		while (node)
		{
			std::wcout << node->data << delim;
			node = node->next;
		}
		std::wcout << L'\n';
	}
	
private:
	Node fakeHead;
	Node*& head = fakeHead.next;

	Node* copy_tail_after(Node* dest, Node* src)
	{
		while (src)
		{
			dest = insert_after(dest, src->data);
			src = src->next;
		}

		return dest;
	}

};


template <typename T>
class LinkedList
{
	struct Node
	{
		T data;
		Node* next = nullptr;
	};

	Node* pHead = nullptr;
	Node* pTail = nullptr;

	size_t length = 0;

public:
	template<typename It>
	LinkedList(It begin, It end)
	{
		auto rbegin = std::make_reverse_iterator(end);
		auto rend = std::make_reverse_iterator(begin);

		for (auto it = rbegin; it != rend; ++it)
			Push(*it);
	}

	~LinkedList()
	{
		while (pHead)
		{
			auto pTemp = pHead;
			pHead = pHead->next;
			delete pTemp;
		}
	}

	bool Empty() const
	{
		return pHead == nullptr;
	}

	size_t Length() const
	{
		return length;
	}

	void Push(const T& val)
	{
		auto pTemp = new Node;
		pTemp->data = val;
		pTemp->next = pHead;
		pHead = pTemp;
		++length;
	}

	T Pop()
	{
		if (Empty())
			throw std::range_error{"the list is empty"};
			
		return Remove(0);
	}

	T Remove(size_t index)
	{
		T val;

		if (index < length)
		{
			size_t pos = 0;
			auto pCur = pHead;

			if (pCur == pHead)
			{
				val = pHead->data;
				pHead = pHead->next;
			}
			else
			{
				while (pos < index - 1)
				{
					pCur = pCur->next;
					++pos;
				}

				val = pCur->next->data;

				auto pTemp = pCur;
				pCur = pCur->next;
				pTemp->next = pCur->next;
			}

			delete pCur;
			--length;
		}
		else
		{ 
			throw std::out_of_range{"invalid index"};
		}
		
		return val;
	}

	T Get(size_t index) const
	{
		T val;

		if (index < length)
		{
			size_t pos = 0;
			auto pCur = pHead;

			while (pos < index)
			{
				pCur = pCur->next;
				++pos;
			}

			val = pCur->data;
		}
		else
		{
			throw std::out_of_range{"invalid index"};
		}
		
		return val;
	}
};