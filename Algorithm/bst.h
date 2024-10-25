#pragma once

// binary search tree

#include <functional>
#include <queue>
#include <type_traits>

template<typename Key, typename Comp = std::less<Key>>
class BST
{
	struct Node
	{
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
		Key key;
	};

	Node* root = nullptr;
	Comp comp;

	void removeSubTree(Node* node)
	{
		if (!node)
			return;

		removeSubTree(node->left);
		removeSubTree(node->right);
		delete node;

	}

	Node* findSubTree(Node* node, const Key& key)
	{
		if (!node)
			return nullptr;

		if (comp(key, node->key))
			return findSubTree(node->left, key);
		else if (comp(node->key, key))
			return findSubTree(node->right, key);
		else
			return node;
	}

public:
	


	BST() = default;
	BST(const BST& src) = delete;	
	BST(BST&& src) = delete;
	BST& operator=(const BST& src) = delete;
	BST& operator=(BST&& src) = delete;

	~BST() 
	{
		clear();
	}

	bool insert(const Key& key)
	{
		Node* node = root;
		Node* parent = nullptr;
		bool isleft = true;

		while (node)
		{
			parent = node;
			if (comp(key, node->key))
			{
				node = node->left;
				isleft = true;
			}
			else if (comp(node->key, key))
			{
				node = node->right;
				isleft = false;
			}
			else
			{
				return false;
			}
		}

		node = new Node;
		node->key = key;
		node->parent = parent;
		if (parent)
			(isleft ? parent->left : parent->right) = node;
		else
			root = node;

		return true;
	}
		
	bool find(const Key& key) { return findSubTree(root, key); }
	
	void remove(const Key& key)
	{
		Node* node = findSubTree(root, key);

		if (!node)
			return;

		if (node->left)
		{
			// search formaximum key in the left subtree 
			
			Node* n = node->left;
			while (n->right)				     
				n = n->right;
											
																		
			(n == node->left ? node->left : n->parent->right) = n->left;
			if (n->left)
				n->left->parent = n->parent;

			node->key = n->key;
			node = n;
			
			// or we could search for minimum in the right subtree
		}
		else	// doesn't have left subtree			
		{
			if (auto p = node->parent)
			{
				(p->left == node ? p->left : p->right) = node->right;
				if (node->right)
					node->right->parent = p;
			}
			else
			{
				if (root = node->right)
					root->parent = nullptr;
			}
		}

		delete node;
		node = nullptr;
	}

	void clear()
	{
		removeSubTree(root);
		root = nullptr;
	}
	
	bool empty() { return !root; }

	template<typename F>
	void dft(F f) const
	{
		dft(root, f);
	}


	// depth-first traversal
	template<typename F>
	void dft(Node* node, F f) const
	{
		if (!node)
			return;

		dft(node->left, f);
		f(node->key);
		dft(node->right, f);
	}

	template<typename F>
	void bft(F f) const
	{
		bft(root, f);
	}

	// breadth-first traversal
	template<typename F>
	void bft(Node* node, F f) const
	{
		if (!node)
			return;

		std::queue<Node*> queue;
		queue.push(node);

		while (!queue.empty())
		{
			if (auto node = queue.front())
			{
				f(node->key);
				queue.push(node->left);
				queue.push(node->right);
			}
			queue.pop();
		}
	}

};
